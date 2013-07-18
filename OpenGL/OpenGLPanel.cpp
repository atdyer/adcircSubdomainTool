#include "OpenGLPanel.h"


OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
	setFocus();
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);

//	layerManager = 0;
//	currentCam = 0;

	viewportWidth = 0.0;
	viewportHeight = 0.0;

	xDomain = 0.0;
	yDomain = 0.0;

	clicking = false;

	viewMode = DisplayMode;

//	connect(&circleTool, SIGNAL(CircleStatsSet(float,float,float)), this, SIGNAL(circleToolStatsSet(float,float,float)));
//	connect(this, SIGNAL(circleToolStatsFinished()), &circleTool, SLOT(CircleFinished()));
//	connect(&circleTool, SIGNAL(NodesSelected(std::vector<Node*>)), &selectionLayer, SLOT(SelectNodes(std::vector<Node*>)));

	// Hook up all of the Selection Layer signals/slots
//	connect(&selectionLayer, SIGNAL(emitMessage(QString)), this, SIGNAL(emitMessage(QString)));
//	connect(&selectionLayer, SIGNAL(numNodesSelected(int)), this, SIGNAL(numNodesSelected(int)));
//	connect(&selectionLayer, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
//	connect(&selectionLayer, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
//	connect(&selectionLayer, SIGNAL(refreshed()), this, SLOT(updateGL()));
//	connect(this, SIGNAL(undo()), &selectionLayer, SLOT(undo()));
//	connect(this, SIGNAL(redo()), &selectionLayer, SLOT(redo()));
}


/**
 * @brief Set the LayerManager object that will perform all drawing operations
 *
 * The LayerManager set using this function will be used to perform all drawing operations. The
 * function LayerManager::DrawVisibleLayers() will be called every time the OpenGL context
 * needs to update.
 *
 * @param newManager The new LayerManager
 */
//void OpenGLPanel::SetLayerManager(LayerManager *newManager)
//{
//	if (newManager)
//	{
//		layerManager = newManager;
//		updateCurrentCamera();
//		connect(layerManager, SIGNAL(activeTerrainLayer(TerrainLayer*)), &circleTool, SLOT(SetTerrainLayer(TerrainLayer*)));
//	}
//}


/**
 * @brief This function is called when the OpenGL context is initialized
 *
 * Called when the OpenGL context is initialized. Use it to load all OpenGL extensions
 * and set all default drawing settings
 *
 */
void OpenGLPanel::initializeGL()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		DEBUG("Error: GLEW unable to load OpenGL Extensions");
		exit(EXIT_FAILURE);
	} else {
		DEBUG("OpenGL Extensions Loaded");
//		cout << "OpenGL Version: "  << glGetString(GL_VERSION) << endl;
//		cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
//              cout << "GLEW Version: " << glewGetString(GLEW_VERSION) << endl;
//              cout << "OpenGL Platform Vendor: " << glGetString(GL_VENDOR) << endl;
//              cout << "OpenGL Renderer: " << glGetString(GL_RENDERER) << endl;
	}

	glClearColor(0.1, 0.2, 0.3, 1.0);
	glDisable(GL_DEPTH_TEST);
//	glEnable(GL_POLYGON_OFFSET_FILL);
//	glEnable(GL_POLYGON_OFFSET_LINE);
//	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(5);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}


/**
 * @brief Called with the size of the OpenGL context changes
 * @param w The new width
 * @param h The new height
 */
void OpenGLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	viewportWidth = w;
	viewportHeight = h;

	if (activeDomain)
		activeDomain->SetWindowSize(w, h);
}


/**
 * @brief Called every time the OpenGL context needs to update the display
 */
void OpenGLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (activeDomain)
		activeDomain->Draw();
}


/**
 * @brief Event fired when the mouse wheel is scrolled
 * @param event
 */
void OpenGLPanel::wheelEvent(QWheelEvent *event)
{

	if (clicking == false && activeDomain)
		activeDomain->Zoom(event->delta());

	updateGL();
}


/**
 * @brief Event fired when a mouse button is clicked
 * @param event
 */
void OpenGLPanel::mousePressEvent(QMouseEvent *event)
{
	clicking = true;
	pushedButton = event->button();

	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;

	if (viewMode == CircleSubdomainMode)
	{
		if (activeDomain)
			activeDomain->SetCircleToolCenter(oldx, oldy);
	}
}


/**
 * @brief Event fired when the mouse is moved over the panel
 * @param event
 */
void OpenGLPanel::mouseMoveEvent(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	emit mouseCoordinates(newx, newy);

	if (clicking)
	{
		if (viewMode == DisplayMode)
		{
			if (activeDomain)
				activeDomain->Pan(dx, dy);
//			if (currentCam)
//				currentCam->Pan(dx, dy);
		}
		else if (viewMode == CircleSubdomainMode)
		{
			if (activeDomain)
				activeDomain->SetCircleToolRadius(newx, newy);
//			circleTool.SetRadiusPoint(event->x(), event->y());
////			emit circleToolStatsSet(circleTool.GetDomainX(), circleTool.GetDomainY(), circleTool.GetRadius());
		}

		updateGL();
	}



	oldx = newx;
	oldy = newy;	
}


/**
 * @brief Event fired when a mouse button is released
 * @param event
 */
void OpenGLPanel::mouseReleaseEvent(QMouseEvent *event)
{
	clicking = false;

	if (!mouseMoved)
	{
		if (viewMode == DisplayMode)
		{
//			float x, y;
//			if (currentCam)
//				currentCam->GetUnprojectedPoint(event->x(), event->y(), &x, &y);
		}
	}

	if (viewMode == CircleSubdomainMode)
	{
		if (activeDomain)
			activeDomain->SetCircleToolFinished();
		viewMode = DisplayMode;
	}

	mouseMoved = false;

	updateGL();
}


void OpenGLPanel::SetActiveDomain(Domain *newDomain)
{
	if (activeDomain)
		disconnect(activeDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
	activeDomain = newDomain;
	activeDomain->SetWindowSize(viewportWidth, viewportHeight);
	connect(activeDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
	connect(this, SIGNAL(mouseCoordinates(float,float)), activeDomain, SLOT(setMouseCoordinates(float,float)));
}


/**
 * @brief Retrieves the pointer to the camera currently being used by the Layer Manager
 * and uses it for drawing.
 */
//void OpenGLPanel::updateCurrentCamera()
//{
//	if (layerManager)
//		currentCam = layerManager->GetCurrentCamera();
//	circleTool.SetCamera(currentCam);
//	selectionLayer.SetCamera(currentCam);
//}


/**
 * @brief Sets the current view mode to Display Mode
 */
void OpenGLPanel::enterDisplayMode()
{
	viewMode = DisplayMode;
}


/**
 * @brief Sets the current view mode to Circle Subdomain Mode
 */
void OpenGLPanel::enterCircleSubdomainMode()
{
	viewMode = CircleSubdomainMode;
}
