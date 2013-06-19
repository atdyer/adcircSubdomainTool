#include "LayerManager.h"

LayerManager::LayerManager(QObject *parent) : QObject(parent)
{
	// Get the LayerThread up and running
	layerThread = new QThread();
	if (layerThread)
	{
		connect(this, SIGNAL(beingDestroyed()), layerThread, SLOT(quit()));
		connect(layerThread, SIGNAL(finished()), layerThread, SLOT(deleteLater()));
		layerThread->start();
	}

	// Initialize the first camera
	currentCam = new GLCamera();
	cameras.push_back(currentCam);
}


LayerManager::~LayerManager()
{
	// Tell the LayerThread it needs to be finish up and then be destroyed
	emit beingDestroyed();

	// Delete all TerrainLayer objects
	for (unsigned int i=0; i<terrainLayers.size(); i++)
	{
		DEBUG("Deleting Terrain Layer. Layer ID: %i\n", terrainLayers[i]->GetID());
		delete terrainLayers[i];
	}

	// Delete all FillShader objects
	for (unsigned int i=0; i<solidShaders.size(); i++)
	{
		DEBUG("Deleting Solid Shader. Shader ID: %i\n", solidShaders[i]->GetID());
		delete solidShaders[i];
	}

	// Delete all GLCamera objects
	for (unsigned int i=0; i<cameras.size(); i++)
	{
		DEBUG("Deleting Camera. Camera ID: %i\n", cameras[i]->GetID());
		delete cameras[i];
	}
}


/**
 * @brief Returns the GLCamera currently being used by the shaders
 * @return Pointer to the current GLCamera
 */
GLCamera* LayerManager::GetCurrentCamera()
{
	return currentCam;
}


/**
 * @brief Changes the GLCamera currently being used by the shaders
 * @param camID The ID of the new GLCamera to use
 */
void LayerManager::SwitchToCamera(unsigned int camID)
{
	for (unsigned int i=0; i<cameras.size(); i++)
	{
		if (cameras[i]->GetID() == camID)
		{
			currentCam = cameras[i];
			UpdateShaderCameras();
			emit cameraChanged();
		}
	}
}


/**
 * @brief Draws all visible layers onto the OpenGL context
 *
 * This function attempts to draw all visible layers onto the OpenGL context.
 *
 */
void LayerManager::DrawVisibleLayers()
{
	for (unsigned int i=0; i<visibleLayers.size(); i++)
		if (visibleLayers[i])
			visibleLayers[i]->Draw();
}


/**
 * @brief Makes a hidden Layer visible
 *
 * This function is used to make a hidden Layer visible without changing the
 * draw order used in visibleLayers.
 *
 * @param layerID The ID of the Layer to show
 */
void LayerManager::ShowLayer(unsigned int layerID)
{
	for (unsigned int i=0; i<hiddenLayers.size(); i++)
		if (hiddenLayers[i] && hiddenLayers[i]->GetID() == layerID)
		{
			visibleLayers[i] = hiddenLayers[i];
			hiddenLayers[i] = 0;
		}
}


/**
 * @brief Makes selected Layer the only one visible
 *
 * This function is used to make the selected layer the only visible Layer without
 * modifying the draw order used in visibleLayers.
 *
 * @param layerID The ID of the Layer to show
 */
void LayerManager::ShowOnlyLayer(unsigned int layerID)
{
	for (unsigned int i=0; i<visibleLayers.size(); i++)
		if (visibleLayers[i] && visibleLayers[i]->GetID() != layerID)
		{
			hiddenLayers[i] = visibleLayers[i];
			visibleLayers[i] = 0;
		}
}


/**
 * @brief Hides a currently visible Layer
 *
 * This function hides a layer that is currently visible without modifying the
 * draw order used in visibleLayers.
 *
 * @param layerID The ID of the Layer to hide
 */
void LayerManager::HideLayer(unsigned int layerID)
{
	for (unsigned int i=0; i<visibleLayers.size(); i++)
		if (visibleLayers[i] && visibleLayers[i]->GetID() == layerID)
		{
			hiddenLayers[i] = visibleLayers[i];
			visibleLayers[i] = 0;
		}
}


/**
 * @brief Creates a new TerrainLayer and prepares it for drawing
 * @param fort14Location The absolute path to the fort.14 file
 *
 * This function creates the new TerrainLayer object, moves the object to the
 * LayerThread so that all slots are executed separately from the GUI thread,
 * and adds the new layer to the list of TerrainLayer objects.
 *
 * @return The new TerrainLayer object's ID
 */
unsigned int LayerManager::CreateNewTerrainLayer(std::string fort14Location, QProgressBar *progressBar)
{
	emit emitMessage(QString::fromStdString("Creating new terrain layer from file: <b>" + fort14Location + "</b>"));

	// Create the new TerrainLayer
	TerrainLayer* newLayer = new TerrainLayer();
	if (!newLayer)
		return 0;

	// Add the new TerrainLayer to the appropriate MEMORY MANAGEMENT list
	terrainLayers.push_back(newLayer);

	// Move the new TerrainLayer to the layerThread
	newLayer->moveToThread(layerThread);

	// Get the layer ready for reading
	connect(newLayer, SIGNAL(fort14Valid()), newLayer, SLOT(readFort14()));
	connect(newLayer, SIGNAL(emitMessage(QString)), this, SIGNAL(emitMessage(QString)));

	// Hook up the progress bar if one has been provided
	if (progressBar)
	{
		connect(newLayer, SIGNAL(startedReadingFort14()), progressBar, SLOT(show()));
		connect(newLayer, SIGNAL(progress(int)), progressBar, SLOT(setValue(int)));
		connect(newLayer, SIGNAL(finishedReadingFort14()), progressBar, SLOT(hide()));
	}

	// Set the fort.14 location to begin reading the file
	newLayer->SetFort14Location(fort14Location);

	// Create a solid outline shader and solid fill shader with some nice default terrain colors
	SolidShader* outlineShader = NewSolidShader(0.2, 0.2, 0.2, 0.5);
	SolidShader* fillShader = NewSolidShader(0.0, 1.0, 0.0, 1.0);

	// Tell the new TerrainLayer to use the newly created shaders
	newLayer->SetOutlineShader(outlineShader);
	newLayer->SetFillShader(fillShader);

	// Create a column for the new TerrainLayer in the REFERENCE TABLE
	// and add all of its attributes to the table as well. Default behavior
	// is to start out visible
	unsigned int newColumn = AddReferenceTableSlot();
	visibleLayers[newColumn] = newLayer;
	solidOutlineShaders[newColumn] = outlineShader;
	solidFillShaders[newColumn] = fillShader;


	// Return the new TerrainLayer's ID
	return newLayer->GetID();
}


/**
 * @brief Tells the Layer to use its solid outline shader
 *
 * Tells the specified Layer object to use the solid outline shader associated with it
 * in the REFERENCE TABLE to draw its outlines. If there is not solid outline shader,
 * one is created, and if the Layer object does not have outlines to draw, the shader
 * is ignored.
 *
 * @param layerID The unique ID of the Layer
 * @param shaderID The unique ID of the GLShader
 */
void LayerManager::UseSolidOutlineShader(unsigned int layerID)
{
	// Find the Shader
	int column = GetReferenceTableColumn(layerID);

	// Check that a column was found
	if (column != -1)
	{
		// If there isn't a solid outline shader for this layer, make one
		if (!solidOutlineShaders[column])
			solidOutlineShaders[column] = NewSolidShader(0.0, 0.0, 0.0, 1.0);

		// Look through only the Layer types that have an outline
		for (unsigned int i=0; i<terrainLayers.size(); i++)
		{
			if (terrainLayers[i]->GetID() == layerID)
			{
				// Tell the TerrainLayer to use the outline shader
				terrainLayers[i]->SetOutlineShader(solidOutlineShaders[column]);
				return;
			}
		}
	}
}


/**
 * @brief Tells the Layer to use its solid fill shader
 *
 * Tells the specified Layer object to use the solid fill shader associated with it
 * in the REFERENCE TABLE to draw its fill. If there is not solid fill shader,
 * one is created, and if the Layer object does not have fill to draw, the shader
 * is ignored.
 *
 * @param layerID The unique ID of the Layer
 * @param shaderID The unique ID of the GLShader
 */
void LayerManager::UseSolidFillShader(unsigned int layerID)
{
	// Find the Shader
	int column = GetReferenceTableColumn(layerID);

	// Check that a column was found
	if (column != -1)
	{
		// If there isn't a solid fill shader for this layer, make one
		if (!solidFillShaders[column])
			solidFillShaders[column] = NewSolidShader(0.0, 0.0, 0.0, 1.0);

		// Look through only the Layer types that have a fill
		for (unsigned int i=0; i<terrainLayers.size(); i++)
		{
			if (terrainLayers[i]->GetID() == layerID)
			{
				// Tell the TerrainLayer to use the fill shader
				terrainLayers[i]->SetOutlineShader(solidFillShaders[column]);
				return;
			}
		}
	}
}


/**
 * @brief Retrieves a pointer to the specified Layer object's solid outline shader
 * @param layerID The desired Layer
 * @return Pointer to the SolidShader object used to draw this Layer object's outline
 * @return 0 if the Layer doesn't exist or if the Layer does't use a solid outline shader
 */
SolidShader* LayerManager::GetSolidOutlineShader(unsigned int layerID)
{
	int column = GetReferenceTableColumn(layerID);

	if (column != -1)
		return solidOutlineShaders[column];
	else
		return 0;
}


/**
 * @brief Retrieves a pointer to the specified Layer object's solid fill shader
 * @param layerID The desired Layer
 * @return Pointer to the SolidShader object used to draw this Layer object's fill
 * @return 0 if the Layer doesn't exist or if the Layer doesn't use a solid fill shader
 */
SolidShader* LayerManager::GetSolidFillShader(unsigned int layerID)
{
	int column = GetReferenceTableColumn(layerID);

	if (column != -1)
		return solidFillShaders[column];
	else
		return 0;
}


/**
 * @brief Adds column to the REFERENCE TABLE for a new Layer
 *
 * This function adds a column to the REFERENCE TABLE that we use for keeping track
 * of associations between Layer objects and GLShader objects. It simply adds a column
 * to the table and returns the integer you can use to access the column in each row.
 *
 * @return The column index
 */
unsigned int LayerManager::AddReferenceTableSlot()
{
	visibleLayers.push_back(0);
	hiddenLayers.push_back(0);
	solidOutlineShaders.push_back(0);
	solidFillShaders.push_back(0);

	// Probably should do some checks here to make sure all vectors are the same size

	return visibleLayers.size()-1;
}


/**
 * @brief Gets the column of the desired Layer in the REFERENCE TABLE
 * @param layerID The unique ID of the desired Layer
 * @return The column of the desired Layer in the REFERENCE TABLE
 * @return -1 if the Layer is not in the REFERENCE TABLE
 */
int LayerManager::GetReferenceTableColumn(unsigned int layerID)
{
	for (unsigned int i=0; i<visibleLayers.size(); i++)
		if (visibleLayers[i] && visibleLayers[i]->GetID() == layerID)
			return i;
	for (unsigned int i=0; i<hiddenLayers.size(); i++)
		if (hiddenLayers[i] && hiddenLayers[i]->GetID() == layerID)
			return i;
	return -1;
}


/**
 * @brief Helper function that looks through the MASS ACCESS for the desired Layer
 * @param layerID The unique ID of the Layer
 * @return A pointer to the Layer
 */
Layer* LayerManager::GetLayerByID(unsigned int layerID)
{
	for (unsigned int i=0; i<terrainLayers.size(); i++)
		if (terrainLayers[i]->GetID() == layerID)
			return terrainLayers[i];
	return 0;
}


/**
 * @brief Helper function that looks through the MASS ACCESS list for the desired GLShader
 * @param shaderID The unique ID of the GLShader
 * @return  A pointer to the GLShader
 */
GLShader* LayerManager::GetShaderByID(unsigned int shaderID)
{
	for (unsigned int i=0; i<allShaders.size(); i++)
		if (allShaders[i]->GetID() == shaderID)
			return allShaders[i];
	return 0;
}



/**
 * @brief Helper function that updates the camera in all GLShader objects to the
 * currently active camera
 */
void LayerManager::UpdateShaderCameras()
{
	for (unsigned int i=0; i<allShaders.size(); i++)
		allShaders[i]->SetCamera(currentCam);
}


/**
 * @brief Helper function that creates a new SolidShader with the desired color
 * @param r Red
 * @param g Green
 * @param b Blue
 * @param a Alpha
 * @return A new SolidShader object owned by the LayerManager
 */
SolidShader* LayerManager::NewSolidShader(float r, float g, float b, float a)
{
	SolidShader* newShader = new SolidShader();
	newShader->SetColor(r, g, b, a);
	newShader->SetCamera(currentCam);

	// Add to MEMORY MANAGEMENT list
	solidShaders.push_back(newShader);

	// Add to MASS ACCESS list
	allShaders.push_back(newShader);


	return newShader;
}
