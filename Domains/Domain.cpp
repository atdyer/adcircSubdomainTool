#include "Domain.h"

/**
 * Constructor that initializes necessary initial class members and hooks up initial signals/slots
 *
 * Every new domain is created with a number of members already initialized. These include a GLCamera object,
 * a SelectionLayer object, a QThread on which to perform all file reading operations, and all of the following tools:
 * - CircleTool
 *
 * All signals from member objects that have been created are connected to the appropriate signals that are emitted by
 * the Domain class. Additionally, all signals from the selection tools are connected to the appropriate slots in the
 * SelectionLayer.
 *
 */
Domain::Domain()
{
	camera = new GLCamera();

	selectionLayer = new SelectionLayer();
	selectionLayer->SetCamera(camera);

	terrainLayer = 0;

	circleTool = new CircleTool();
	circleTool->SetCamera(camera);

	layerThread = new QThread();
	if (layerThread)
	{
		connect(this, SIGNAL(beingDestroyed()), layerThread, SLOT(quit()));
		connect(layerThread, SIGNAL(finished()), layerThread, SLOT(deleteLater()));
		layerThread->start();
	}

	progressBar = 0;
	loadingLayer = 0;


	/* Connect all selection tools to the selection layer */
	connect(circleTool, SIGNAL(NodesSelected(std::vector<Node*>)), selectionLayer, SLOT(SelectNodes(std::vector<Node*>)));
	connect(circleTool, SIGNAL(NodeSelected(Node*)), selectionLayer, SLOT(SelectNode(Node*)));

	/* Pass signals up from the selection layer */
	connect(selectionLayer, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
	connect(selectionLayer, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
	connect(selectionLayer, SIGNAL(emitMessage(QString)), this, SIGNAL(emitMessage(QString)));
	connect(selectionLayer, SIGNAL(numNodesSelected(int)), this, SIGNAL(numNodesSelected(int)));
	connect(selectionLayer, SIGNAL(numElementsSelected(int)), this, SIGNAL(numElementsSelected(int)));

	/* Pass signals up from all selection tools */
	connect(circleTool, SIGNAL(CircleStatsSet(float,float,float)), this, SIGNAL(circleToolStatsSet(float,float,float)));
	connect(circleTool, SIGNAL(CircleStatsFinished()), this, SIGNAL(circleToolStatsFinished()));

}


/**
 * @brief Destructor that deletes all member objects that have been created using new.
 *
 * Each Domain object owns all of the Layers, Tools, Cameras, etc. that it uses, and it
 * is responsible for cleaning up all of that data.
 *
 */
Domain::~Domain()
{
	if (selectionLayer)
		delete selectionLayer;
	if (terrainLayer)
		delete terrainLayer;

	if (circleTool)
		delete circleTool;
}


/**
 * @brief Draws all visible data
 *
 * Draws all visible data. This function determines the order that Layers are drawn in. The first Layer
 * drawn will be on the bottom, the second will be above the first, and so on.
 *
 */
void Domain::Draw()
{
	if (terrainLayer)
		terrainLayer->Draw();
	if (selectionLayer)
		selectionLayer->Draw();
	if (circleTool)
		circleTool->Draw();
}


/**
 * @brief Zooms the view by calling the Zoom function of the camera
 *
 * Zooms the view by calling the Zoom function of the camera
 *
 * @param zoomAmount A positive value for zooming in, a negative value for zooming out
 */
void Domain::Zoom(float zoomAmount)
{
	if (camera)
		camera->Zoom(zoomAmount);
}


/**
 * @brief Pans the view by calling the Pan function of the camera
 *
 * Pans the view by calling the Pan function of the camera
 *
 * @param dx Change in x (pixels)
 * @param dy Change in y (pixels)
 */
void Domain::Pan(float dx, float dy)
{
	if (camera)
		camera->Pan(dx, dy);
}


/**
 * @brief Sets the window size used by the camera and all tools
 *
 * Sets the OpenGL viewport size used by the camera and all selection tools
 *
 * @param w Width (pixels)
 * @param h Height (pixels)
 */
void Domain::SetWindowSize(float w, float h)
{
	if (camera)
		camera->SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -1000.0, 1000.0);

	if (circleTool)
		circleTool->SetViewportSize(w, h);
}


/**
 * @brief Sets the center point of the circle used in the CircleTool selection tool
 *
 * Sets the center point of the circle used in the CircleTool selection tool
 *
 * @param x The x-coordinate (pixels)
 * @param y The y-coordinate (pixels)
 */
void Domain::SetCircleToolCenter(int x, int y)
{
	if (circleTool)
		circleTool->SetCenter(x, y);
}


/**
 * @brief Sets the point used to calculate the radius of the circle used in the
 * CircleTool selection tool
 *
 * Sets the point used to calculate the radius of the circle used in the
 * CircleTool selection tool
 *
 * @param x The x-coordinate (pixels)
 * @param y The y-coordinate (pixels)
 */
void Domain::SetCircleToolRadius(int x, int y)
{
	if (circleTool)
		circleTool->SetRadiusPoint(x, y);
}


/**
 * @brief Tells the CircleTool that the user has finished drawing the circle
 *
 * Tells the CircleTool that the user has finished drawing the circle. This will
 * initialize actual selection of Nodes/Elements inside of the circle.
 *
 */
void Domain::SetCircleToolFinished()
{
	if (circleTool)
		circleTool->CircleFinished();
}


/**
 * @brief Undoes the last selection action performed by the user
 *
 * Undoes the last selection action performed by the user
 *
 */
void Domain::Undo()
{
	if (selectionLayer)
		selectionLayer->undo();
	emit updateGL();
}


/**
 * @brief Redoes the last undo performed by the user
 *
 * Redoes the last undo performed by the user
 *
 */
void Domain::Redo()
{
	if (selectionLayer)
		selectionLayer->redo();
	emit updateGL();
}


/**
 * @brief Sets the progress bar that will be used to display progress of all
 * file reading operations
 *
 * Sets the progress bar that will be used to display progress of all
 * file reading operations. Does not take ownership of the QProgressBar.
 *
 * @param newBar Pointer to a progress bar in the user interface
 */
void Domain::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;
}


/**
 * @brief Sets the fort.14 file location used by the TerrainLayer
 *
 * Sets the fort.14 file location used by the TerrainLayer. If a terrain layer
 * has not been created yet, a new one will be created and connected to the Domain's
 * signal/slot mechanism.
 *
 * Default colors are used for drawing the terrain if a new layer has to be created.
 * Otherwise, all colors are kept the same and the new file is loaded.
 *
 * @param newLoc The fort.14 file location
 */
void Domain::SetFort14Location(std::string newLoc)
{
	if (!terrainLayer)
	{
		terrainLayer = new TerrainLayer();
		loadingLayer = terrainLayer;
		terrainLayer->moveToThread(layerThread);

		if (progressBar)
		{
			connect(terrainLayer, SIGNAL(startedReadingData()), progressBar, SLOT(show()));
			connect(terrainLayer, SIGNAL(progress(int)), progressBar, SLOT(setValue(int)));
			connect(terrainLayer, SIGNAL(finishedReadingData()), progressBar, SLOT(hide()));
		}

		connect(terrainLayer, SIGNAL(emitMessage(QString)), this, SIGNAL(emitMessage(QString)));
		connect(terrainLayer, SIGNAL(finishedReadingData()), this, SLOT(LoadLayerToGPU()));
		connect(terrainLayer, SIGNAL(finishedLoadingToGPU()), this, SIGNAL(updateGL()));
		connect(terrainLayer, SIGNAL(foundNumNodes(int)), this, SIGNAL(numNodesDomain(int)));
		connect(terrainLayer, SIGNAL(foundNumElements(int)), this, SIGNAL(numElementsDomain(int)));


		terrainLayer->SetCamera(camera);
		terrainLayer->SetSolidOutline(SolidShaderProperties(0.2, 0.2, 0.2, 0.1));
		terrainLayer->SetSolidFill(SolidShaderProperties(0.1, 0.8, 0.1, 1.0));
		terrainLayer->SetSolidBoundary(SolidShaderProperties(0.0, 0.0, 0.0, 1.0));
//		terrainLayer->SetGradientOutline(GradientShaderProperties(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0));
//		terrainLayer->SetGradientFill(GradientShaderProperties(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0));

		if (circleTool)
			circleTool->SetTerrainLayer(terrainLayer);
	}
	terrainLayer->SetFort14Location(newLoc);
}


/**
 * @brief Sets the fort.15 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.15 file location
 */
void Domain::SetFort15Location(std::string newLoc)
{

}


/**
 * @brief Sets the fort.63 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.63 file location
 */
void Domain::SetFort63Location(std::string newLoc)
{

}


/**
 * @brief Sets the fort.64 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.64 file location
 */
void Domain::SetFort64Location(std::string newLoc)
{

}


/**
 * @brief Sets the properties used to draw a solid outline in the terrain layer
 *
 * Sets the properties used to draw a solid outline in the terrain layer. If the
 * terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainSolidOutline(SolidShaderProperties newProperties)
{
	if (terrainLayer)
		terrainLayer->SetSolidOutline(newProperties);

}


/**
 * @brief Sets the properties used to draw a solid fill in the terrain layer
 *
 * Sets the properties used to draw a solid fill in the terrain layer. If the
 * terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainSolidFill(SolidShaderProperties newProperties)
{
	if (terrainLayer)
		terrainLayer->SetSolidFill(newProperties);
}


/**
 * @brief Sets the properties used to draw a gradient outline in the terrain layer
 *
 * Sets the properties used to draw a gradient outline in the terrain layer.
 * If the terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainGradientOutline(GradientShaderProperties newProperties)
{
	if (terrainLayer)
		terrainLayer->SetGradientOutline(newProperties);
}


/**
 * @brief Sets the properties used to draw a gradient fill in the terrain layer
 *
 * Sets the properties used to draw a gradient fill in the terrain layer.
 * If the terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainGradientFill(GradientShaderProperties newProperties)
{
	if (terrainLayer)
		terrainLayer->SetGradientFill(newProperties);
}


/**
 * @brief Returns the fort.14 file location being used by the terrain layer
 *
 * Returns the fort.14 file location being used by the terrain layer
 *
 * @return The fort.14 file location being used by the terrain layer
 */
std::string Domain::GetFort14Location()
{
	if (terrainLayer)
		return terrainLayer->GetFort14Location();
	return std::string("");
}


/**
 * @brief Returns the fort.15 file location
 *
 * <b>Not yet implemented. Returns an empty string</b>
 *
 * @return The fort.15 file location
 */
std::string Domain::GetFort15Location()
{
	return std::string("");
}


/**
 * @brief Returns the fort.63 file location
 *
 * <b>Not yet implemented. Returns an empty string</b>
 *
 * @return The fort.63 file location
 */
std::string Domain::GetFort63Location()
{
	return std::string("");
}


/**
 * @brief Returns the fort.64 file location
 *
 * <b>Not yet implemented. Returns an empty string</b>
 *
 * @return The fort.64 file location
 */
std::string Domain::GetFort64Location()
{
	return std::string("");
}


/**
 * @brief Returns the properties of the terrain layer's solid outline
 *
 * The properties of the terrain layer's solid outline. If the terrain layer
 * has not been created yet, default properties are returned.
 *
 * @return The properties of the terrain layer's solid outline
 * @return Default solid outline properties if the terrain layer has not been created
 */
SolidShaderProperties Domain::GetTerrainSolidOutline()
{
	if (terrainLayer)
		return terrainLayer->GetSolidOutline();
	return SolidShaderProperties();
}


/**
 * @brief Returns the properties of the terrain layer's solid fill
 *
 * Returns the properties of the terrain layer's solid fill. If the terrain layer
 * has not been created yet, default properties are returned.
 *
 * @return The properties of the terrain layer's solid fill
 * @return Default solid fill properties if the terrain layer has not been created
 */
SolidShaderProperties Domain::GetTerrainSolidFill()
{
	if (terrainLayer)
		return terrainLayer->GetSolidFill();
	return SolidShaderProperties();
}


/**
 * @brief Returns the properties of the terrain layer's gradient outline
 *
 * Returns the properties of the terrain layer's gradient outline. If the terrain layer
 * has not been created yet, default properties are returned.
 *
 * @return The properties of the terrain layer's gradient outline
 * @return Default gradient outline properties if the terrain layer has not been created
 */
GradientShaderProperties Domain::GetTerrainGradientOutline()
{
	if (terrainLayer)
		return terrainLayer->GetGradientOutline();
	return GradientShaderProperties();
}


/**
 * @brief Returns the properties of the terrain layer's gradient fill
 *
 * Returns the properties of the terrain layer's gradient fill. If the terrain layer
 * has not been created yet, default properties are returned.
 *
 * @return The properties of the terrain layer's gradient fill
 * @return Default gradient fill properties if the terrain layer has not been created
 */
GradientShaderProperties Domain::GetTerrainGradientFill()
{
	if (terrainLayer)
		return terrainLayer->GetGradientFill();
	return GradientShaderProperties();
}


/**
 * @brief Returns the number of nodes defined by the terrain layer
 *
 * Returns the number of nodes defined by the terrain layer. If the terrain layer
 * has not been created, zero is returned.
 *
 * @return The number of nodes in the terrain layer
 * @return 0 if the terrain layer has not been created
 */
unsigned int Domain::GetNumNodesDomain()
{
	if (terrainLayer)
		return terrainLayer->GetNumNodes();
	return 0;
}


/**
 * @brief Returns the number of elements defined by the terrain layer
 *
 * Returns the number of elements defined by the terrain layer. If the terrain layer
 * has not been created, zero is returned.
 *
 * @return The number of elements in the terrain layer
 * @return 0 if the terrain layer has not been created
 */
unsigned int Domain::GetNumElementsDomain()
{
	if (terrainLayer)
		return terrainLayer->GetNumElements();
	return 0;
}


/**
 * @brief Returns the number of nodes currently selected
 *
 * Returns the number of nodes currently selected in the selection layer. If the
 * selection layer has not been created, zero is returned.
 *
 * @return The number of nodes currently selected in the selection layer
 */
unsigned int Domain::GetNumNodesSelected()
{
	if (selectionLayer)
		return selectionLayer->GetNumNodesSelected();
	return 0;
}


/**
 * @brief Returns the number of elements currently selected
 *
 * Returns the number of elements currently selected in the selection layer. If the
 * selection layer has not been created, zero is returned.
 *
 * @return The number of elements currently selected in the selection layer
 */
unsigned int Domain::GetNumElementsSelected()
{
	if (selectionLayer)
		return selectionLayer->GetNumElementsSelected();
	return 0;
}


/**
 * @brief Sets the mouse coordinates in pixels so that transformations can be
 * performed internally to get the mouse coordinates in the domain's coordinate
 * system
 *
 * Sets the mouse coordinates in pixels so that transformations can be
 * performed internally to get the mouse coordinates in the domain's coordinate
 * system. Implemented as a slot in order to more easily connect to the OpenGLPanel
 * class for continuous display of mouse x-y coordinates.
 *
 * Causes the mouseX and mouseY signals to be fired. If the terrain layer has been
 * created (and thus, the domain's coordinate system is known), those signals will contain
 * mouse coordinates in domain space. Otherwise, they will be in either normalized OpenGL
 * space (the camera has been created), or in pixels (the camera has not been created).
 *
 * @param x The mouse x-coordinate (pixels)
 * @param y The mouse y-coordinate (pixels)
 */
void Domain::setMouseCoordinates(float x, float y)
{
	if (camera)
	{
		float glX, glY, domX, domY;
		camera->GetUnprojectedPoint(x, y, &glX, &glY);
		if (terrainLayer)
		{
			domX = terrainLayer->GetUnprojectedX(glX);
			domY = terrainLayer->GetUnprojectedY(glY);
			emit mouseX(domX);
			emit mouseY(domY);
		} else {
			emit mouseX(glX);
			emit mouseY(glY);
		}
	} else {
		emit mouseX(x);
		emit mouseY(y);
	}
}


/**
 * @brief This function is used internally to load Layer data to the GPU
 *
 * This function is used internally to load Layer data to the GPU. Layer data is
 * read from file using a separate thread. However, that data can only be loaded
 * to the OpenGL context from the main thread. Therefore, we provide this internal
 * slot mechanism that needs to be connected to the finishedReadingData() signal
 * of a Layer that is reading a file on a different thread. Once that signal
 * has been triggered, this function explicitly calls the LoadDataToGPU() function
 * of the Layer, causing it to be executed on the main thread.
 *
 * <b>NOTE TO SELF: Should probably use a queue instead of a single pointer in case
 * the user opens another file before the first finishes loading.</b>
 *
 */
void Domain::LoadLayerToGPU()
{
	if (loadingLayer)
	{
		loadingLayer->LoadDataToGPU();
		disconnect(loadingLayer, SIGNAL(finishedReadingData()), this, SLOT(LoadLayerToGPU()));
		loadingLayer = 0;
	}
}


