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
}


LayerManager::~LayerManager()
{
	// Tell the LayerThread it needs to be finish up and then be destroyed
	emit beingDestroyed();

	// Delete all TerrainLayer objects
	for (unsigned int i=0; i<terrainLayers.size(); i++)
	{
		std::cout << "Deleting Terrain Layer" << std::endl;
		delete terrainLayers[i];
	}
}


/**
 * @brief This function must be called before any drawing will occur
 * @param panel The OpenGLPanel on the UI that containts the OpenGL context
 */
void LayerManager::Initialize(OpenGLPanel *panel)
{
	if (panel)
	{
		GLPanel = panel;

		// Create the first camera to be used
		GLCamera* newCam = new GLCamera();
		GLPanel->SetCamera(newCam);
		currentCam = newCam;
		cameras.push_back(newCam);
	}
}


void LayerManager::SwitchToCamera(unsigned int cam)
{

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
 * @brief Tells the LayerManager to start drawing a Layer that is not currently begin drawn
 *
 * Use this function to begin drawing a Layer that is neither visible nor hidden. It adds
 * the Layer to the visibleLayers list as well as a spot for it in the hiddenLayers list.
 *
 * @param layerID The ID of the Layer to display
 */
void LayerManager::DisplayLayer(unsigned int layerID)
{
	visibleLayers.push_back(GetLayerByID(layerID));
	hiddenLayers.push_back(0);
}


/**
 * @brief Tells the LayerManager to only draw the specified Layer
 *
 * Use this function to clear the view and draw only the specified Layer
 *
 * @param layerID The ID of the Layer to display
 */
void LayerManager::DisplayLayerUnique(unsigned int layerID)
{
	visibleLayers.clear();
	hiddenLayers.clear();
	DisplayLayer(layerID);
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
	// This test should always pass
	if (visibleLayers.size() == hiddenLayers.size())
		for (unsigned int i=0; i<hiddenLayers.size(); i++)
			if (hiddenLayers[i]->GetID() == layerID && visibleLayers[i] == 0)
			{
				visibleLayers[i] = hiddenLayers[i];
				hiddenLayers[i] = 0;
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
	// This test should always pass
	if (visibleLayers.size() == hiddenLayers.size())
		for (unsigned int i=0; i<visibleLayers.size(); i++)
			if (visibleLayers[i]->GetID() == layerID && hiddenLayers[i] == 0)
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

	// Add the new TerrainLayer to the list of TerrainLayers
	terrainLayers.push_back(newLayer);

	// Create a solid outline shader and solid fill shader with some nice default terrain colors
	SolidShader* outlineShader = new SolidShader();
	SolidShader* fillShader = new SolidShader();
	outlineShader->SetColor(0.2, 0.2, 0.2, 0.5);
	fillShader->SetColor(0.0, 1.0, 0.0, 1.0);

	// Tell the new shaders to use the current camera
	outlineShader->SetCamera(currentCam);
	fillShader->SetCamera(currentCam);

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
 * @brief Helper function that looks through all of the lists for the desired GLShader
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


GLShader* LayerManager::GetShaderByID(unsigned int shaderID)
{
	for (unsigned int i=0; i<solidShaders.size(); i++)
		if (solidShaders[i]->GetID() == shaderID)
			return solidShaders[i];
	return 0;
}
