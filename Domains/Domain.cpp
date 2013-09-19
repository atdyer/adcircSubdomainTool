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
	/***** Perform Initializations *****/
	camera = new GLCamera();

	terrainLayer = 0;
	selectionLayer = new CreationSelectionLayer();

	layerThread = new QThread();
	progressBar = 0;
	loadingLayer = 0;

	currentMode = DisplayAction;
	oldx = oldy = newx = newy = dx = dy = 0;
	pushedButton = Qt::LeftButton;
	clicking = mouseMoved = false;

	domainPath = "";
	fort14Location = "";
	fort15Location = "";
	fort63Location = "";
	fort64Location = "";
	py140Location = "";



	/***** Perform Required Initial Function Calls *****/
	if (selectionLayer && camera)
		selectionLayer->SetCamera(camera);

	if (layerThread)
	{
		connect(this, SIGNAL(BeingDestroyed()), layerThread, SLOT(quit()));
		connect(layerThread, SIGNAL(finished()), layerThread, SLOT(deleteLater()));
		layerThread->start();
	}

	/* Pass signals up from the selection layer */
	connect(selectionLayer, SIGNAL(Message(QString)), this, SIGNAL(Message(QString)));
	connect(selectionLayer, SIGNAL(Instructions(QString)), this, SIGNAL(Instructions(QString)));
	connect(selectionLayer, SIGNAL(UndoAvailable(bool)), this, SIGNAL(UndoAvailable(bool)));
	connect(selectionLayer, SIGNAL(RedoAvailable(bool)), this, SIGNAL(RedoAvailable(bool)));
	connect(selectionLayer, SIGNAL(Refreshed()), this, SIGNAL(UpdateGL()));
	connect(selectionLayer, SIGNAL(NumElementsSelected(int)), this, SIGNAL(NumElementsSelected(int)));
	connect(selectionLayer, SIGNAL(CircleToolStatsSet(float,float,float)), this, SIGNAL(CircleToolStatsSet(float,float,float)));
	connect(selectionLayer, SIGNAL(RectangleToolStatsSet(float,float)), this, SIGNAL(RectangleToolStatsSet(float,float)));
	connect(selectionLayer, SIGNAL(ToolFinishedDrawing()), this, SIGNAL(ToolFinishedDrawing()));

	connect(selectionLayer, SIGNAL(ToolFinishedDrawing()), this, SLOT(EnterDisplayMode()));

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
	if (camera)
		delete camera;
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
	{
		if (!terrainLayer->DataLoaded() && !fort14Location.isEmpty())
		{
			LoadFort14File();
		}
		terrainLayer->Draw();
	}
	if (selectionLayer)
		selectionLayer->Draw();
}


void Domain::MouseClick(QMouseEvent *event)
{
	clicking = true;
	pushedButton = event->button();

	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->MouseClick(event);

	emit UpdateGL();
}


void Domain::MouseMove(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	CalculateMouseCoordinates();

	if (currentMode == DisplayAction)
	{
		if (clicking)
		{
			Pan(dx, dy);
			emit UpdateGL();
		}
	}
	else if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseMove(event);
		emit UpdateGL();
	}

	oldx = newx;
	oldy = newy;
}


void Domain::MouseRelease(QMouseEvent *event)
{
	clicking = false;
	mouseMoved = false;

	oldx = event->x();
	oldy = event->y();

	if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseRelease(event);
	}

	emit UpdateGL();
}


void Domain::MouseWheel(QWheelEvent *event)
{
	if (!clicking)
		Zoom(event->delta());

	CalculateMouseCoordinates();

	emit UpdateGL();
}


void Domain::KeyPress(QKeyEvent *event)
{
	if (event->key() == Qt::Key_O)
		ToggleTerrainQuadtree();
	else if (event->key() == Qt::Key_E)
		UseTool(ClickToolType, ElementSelection);

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->KeyPress(event);
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

	if (selectionLayer)
		selectionLayer->WindowSizeChanged(w, h);
}


void Domain::UseTool(ToolType tool, SelectionType selection)
{
	currentMode = SelectionAction;
	if (selectionLayer)
		selectionLayer->UseTool(tool, selection);
	emit SetCursor(Qt::CrossCursor);
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
		selectionLayer->Undo();
	emit UpdateGL();
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
		selectionLayer->Redo();
	emit UpdateGL();
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


void Domain::SetDomainPath(QString newPath)
{
	domainPath = newPath;
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
void Domain::SetFort14Location(QString newLoc)
{
	fort14Location = newLoc;
	CreateTerrainLayer();
}


/**
 * @brief Sets the fort.15 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.15 file location
 */
void Domain::SetFort15Location(QString newLoc)
{
	fort15Location = newLoc;
}


/**
 * @brief Sets the fort.63 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.63 file location
 */
void Domain::SetFort63Location(QString newLoc)
{
	fort63Location = newLoc;
}


/**
 * @brief Sets the fort.64 file location
 *
 * <b>Not yet implemented</b>
 *
 * @param newLoc The fort.64 file location
 */
void Domain::SetFort64Location(QString newLoc)
{
	fort64Location = newLoc;
}


void Domain::SetPy140Location(QString newLoc)
{
	py140Location = newLoc;
}


/**
 * @brief Sets the properties used to draw a solid outline in the terrain layer
 *
 * Sets the properties used to draw a solid outline in the terrain layer. If the
 * terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainSolidOutline(QColor newColor)
{
	if (terrainLayer)
		terrainLayer->SetSolidOutline(newColor);

	emit UpdateGL();

}


/**
 * @brief Sets the properties used to draw a solid fill in the terrain layer
 *
 * Sets the properties used to draw a solid fill in the terrain layer. If the
 * terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainSolidFill(QColor newColor)
{
	if (terrainLayer)
		terrainLayer->SetSolidFill(newColor);

	emit UpdateGL();
}


/**
 * @brief Sets the properties used to draw a gradient outline in the terrain layer
 *
 * Sets the properties used to draw a gradient outline in the terrain layer.
 * If the terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainGradientOutline(QGradientStops newStops)
{
	if (terrainLayer)
		terrainLayer->SetGradientOutline(newStops);

	emit UpdateGL();
}


/**
 * @brief Sets the properties used to draw a gradient fill in the terrain layer
 *
 * Sets the properties used to draw a gradient fill in the terrain layer.
 * If the terrain layer has not been created, it will be ignored.
 *
 * @param newProperties The shader properties
 */
void Domain::SetTerrainGradientFill(QGradientStops newStops)
{
	if (terrainLayer)
		terrainLayer->SetGradientFill(newStops);

	emit UpdateGL();
}


QString Domain::GetDomainPath()
{
	return domainPath;
}


/**
 * @brief Returns the fort.14 file location
 *
 * Returns the fort.14 file location
 *
 * @return The fort.14 file location
 */
QString Domain::GetFort14Location()
{
	return fort14Location;
}


/**
 * @brief Returns the fort.15 file location
 *
 * Returns the fort.15 file location
 *
 * @return The fort.15 file location
 */
QString Domain::GetFort15Location()
{
	return fort15Location;
}


/**
 * @brief Returns the fort.63 file location
 *
 * Returns the fort.63 file location
 *
 * @return The fort.63 file location
 */
QString Domain::GetFort63Location()
{
	return fort63Location;
}


/**
 * @brief Returns the fort.64 file location
 *
 * Returns the fort.64 file location
 *
 * @return The fort.64 file location
 */
QString Domain::GetFort64Location()
{
	return fort64Location;
}


QString Domain::GetPy140Location()
{
	return py140Location;
}


std::vector<Element>* Domain::GetAllElements()
{
	return terrainLayer->GetAllElements();
}


ElementState* Domain::GetCurrentSelectedElements()
{
	if (selectionLayer)
	{
		return selectionLayer->GetCurrentSelection();
	}
	return 0;
}


float Domain::GetTerrainMinElevation()
{
	if (terrainLayer)
		return terrainLayer->GetMinZ();
	return 0.0;
}


float Domain::GetTerrainMaxElevation()
{
	if (terrainLayer)
		return terrainLayer->GetMaxZ();
	return 1.0;
}


ShaderType Domain::GetTerrainOutlineShaderType()
{
	if (terrainLayer)
		return terrainLayer->GetOutlineShaderType();
	return NoShaderType;
}


ShaderType Domain::GetTerrainFillShaderType()
{
	if (terrainLayer)
		return terrainLayer->GetFillShaderType();
	return NoShaderType;
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
QColor Domain::GetTerrainSolidOutline()
{
	if (terrainLayer)
		return terrainLayer->GetSolidOutline();
	return QColor();
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
QColor Domain::GetTerrainSolidFill()
{
	if (terrainLayer)
		return terrainLayer->GetSolidFill();
	return QColor();
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
QGradientStops Domain::GetTerrainGradientOutline()
{
	if (terrainLayer)
		return terrainLayer->GetGradientOutline();
	return QGradientStops();
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
QGradientStops Domain::GetTerrainGradientFill()
{
	if (terrainLayer)
		return terrainLayer->GetGradientFill();
	return QGradientStops();
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


GLCamera* Domain::GetCamera()
{
	return camera;
}


/**
 * @brief Displays or hides the Quadtree of the TerrainLayer
 *
 * Displays or hides the Quadtree of the TerrainLayer
 *
 * @param toggleValue true to display the Quadtree, false to hide it
 */
void Domain::ToggleTerrainQuadtree()
{
	if (terrainLayer)
	{
		terrainLayer->ToggleQuadtreeVisible();
		emit UpdateGL();
	}
}


void Domain::LoadFort14File()
{
	CreateTerrainLayer();
	loadingLayer = terrainLayer;
	terrainLayer->moveToThread(layerThread);
	terrainLayer->SetData(fort14Location);
}


/**
 * @brief Creates the TerrainLayer object if one does not already exist
 *
 * Creates the TerrainLayer object if one does not already exist. Uses default
 * color values and connect all signals/slots. Does set the data used by the
 * Layer.
 *
 */
void Domain::CreateTerrainLayer()
{
	if (!terrainLayer)
	{
		terrainLayer = new TerrainLayer();

		if (progressBar)
		{
			connect(terrainLayer, SIGNAL(startedReadingData()), progressBar, SLOT(show()));
			connect(terrainLayer, SIGNAL(progress(int)), progressBar, SLOT(setValue(int)));
			connect(terrainLayer, SIGNAL(finishedReadingData()), progressBar, SLOT(hide()));
		}

		connect(terrainLayer, SIGNAL(EmitMessage(QString)), this, SIGNAL(Message(QString)));
		connect(terrainLayer, SIGNAL(finishedReadingData()), this, SLOT(LoadLayerToGPU()));
		connect(terrainLayer, SIGNAL(finishedLoadingToGPU()), this, SIGNAL(UpdateGL()));
		connect(terrainLayer, SIGNAL(foundNumNodes(int)), this, SIGNAL(NumNodesDomain(int)));
		connect(terrainLayer, SIGNAL(foundNumElements(int)), this, SIGNAL(NumElementsDomain(int)));


		terrainLayer->SetCamera(camera);
		terrainLayer->SetSolidOutline(QColor(0.2*255, 0.2*255, 0.2*255, 0.1*255));
		terrainLayer->SetSolidFill(QColor(0.1*255, 0.8*255, 0.1*255, 1.0*255));
		terrainLayer->SetSolidBoundary(QColor(0.0*255, 0.0*255, 0.0*255, 1.0*255));
//		terrainLayer->SetGradientOutline(GradientShaderProperties(0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 1.0, 0.0, 1.0));
//		terrainLayer->SetGradientFill(GradientShaderProperties(0.0, 0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0, 1.0));

		if (selectionLayer)
			selectionLayer->SetTerrainLayer(terrainLayer);
	}
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
	if (terrainLayer)
		terrainLayer->UpdateZoomLevel(zoomAmount);
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
 * @brief Calculates the mouse coordinates (based on newx and newy) in the domain's
 * coordinate system
 *
 * Calculates the mouse coordinates (based on newx and newy) in the domain's
 * coordinate system. Fires the mouseX and mouseY signals with the calculated
 * values. If no camera has been set, these are pixel values. If a camera has
 * been set but a terrain layer hasn't, these are in the OpenGL coordinate
 * system. If both have been set, these values are in the terrain layer's
 * coordinate system.
 *
 */
void Domain::CalculateMouseCoordinates()
{
	if (camera)
	{
		float glX, glY, domX, domY;
		camera->GetUnprojectedPoint(newx, newy, &glX, &glY);
		if (terrainLayer)
		{
			domX = terrainLayer->GetUnprojectedX(glX);
			domY = terrainLayer->GetUnprojectedY(glY);
			emit MouseX(domX);
			emit MouseY(domY);
		} else {
			emit MouseX(glX);
			emit MouseY(glY);
		}
	} else {
		emit MouseX(newx);
		emit MouseY(newy);
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


void Domain::EnterDisplayMode()
{
	currentMode = DisplayAction;
	emit SetCursor(Qt::ArrowCursor);
}
