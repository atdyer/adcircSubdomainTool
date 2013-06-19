#include "LayerManager.h"

LayerManager::LayerManager(QObject *parent) : QObject(parent)
{
	// Get the LayerThread up and running
	LayerThread = new QThread();
	if (LayerThread)
	{
		connect(this, SIGNAL(beingDestroyed()), LayerThread, SLOT(quit()));
		connect(LayerThread, SIGNAL(finished()), LayerThread, SLOT(deleteLater()));
		LayerThread->start();
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
		DEBUG("Deleting Terrain Layer\n");
		delete terrainLayers[i];
	}

	// Delete all FillShader objects
	for (unsigned int i=0; i<solidShaders.size(); i++)
	{
		DEBUG("Deleting Solid Shader\n");
		delete solidShaders[i];
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
		if (hiddenLayers[i]->GetID() == layerID)
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
		if (visibleLayers[i]->GetID() != layerID)
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
		if (visibleLayers[i]->GetID() == layerID)
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

	// Move the new TerrainLayer to the LayerThread
	newLayer->moveToThread(LayerThread);

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

	// Add the new TerrainLayer to the list of TerrainLayers and visible layers
	terrainLayers.push_back(newLayer);
	visibleLayers.push_back(newLayer);
	hiddenLayers.push_back(0);

	// Create a solid outline shader and solid fill shader with some nice default terrain colors
	SolidShader* outlineShader = NewSolidShader(0.2, 0.2, 0.2, 0.5);
	SolidShader* fillShader = NewSolidShader(0.0, 1.0, 0.0, 1.0);

	// Tell the new TerrainLayer to use the newly created shaders
	newLayer->SetOutlineShader(outlineShader);
	newLayer->SetFillShader(fillShader);

	// Return the new TerrainLayer's ID
	return newLayer->GetID();
}


/**
 * @brief Assigns the desired shader to be used in drawing the outline of the desired layer
 * @param layerID The unique ID of the layer
 * @param shaderID The unique ID of the shader
 */
void LayerManager::PairOutlineShader(unsigned int layerID, unsigned int shaderID)
{
	// Find the Shader
	GLShader* targetShader = GetShaderByID(shaderID);

	// Find the Layer if a Shader was found
	if (targetShader)
	{
		// Only look through Layer types that have an outline
		for (unsigned int i=0; i<terrainLayers.size(); i++)
			if (terrainLayers[i]->GetID() == layerID)
			{
				terrainLayers[i]->SetOutlineShader(targetShader);
			}
	}
}


/**
 * @brief Assigns the desired shader to be used in drawing the fill of the desired layer
 * @param layerID The unique ID of the layer
 * @param shaderID The unique ID of the shader
 */
void LayerManager::PairFillShader(unsigned int layerID, unsigned int shaderID)
{
	// Find the Shader
	GLShader* targetShader = GetShaderByID(shaderID);

	// Find the Layer if a Shader was found
	if (targetShader)
	{
		// Only look through Layer types that have a fill
		for (unsigned int i=0; i<terrainLayers.size(); i++)
			if (terrainLayers[i]->GetID() == layerID)
			{
				terrainLayers[i]->SetFillShader(targetShader);
			}
	}
}


/**
 * @brief Helper function that looks through all of the lists for the desired Layer
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
 * @brief Helper function that looks through the list of all shaders for the desired GLShader
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

	allShaders.push_back(newShader);
	solidShaders.push_back(newShader);

	return newShader;
}
