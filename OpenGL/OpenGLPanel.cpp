#include "OpenGLPanel.h"


OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
	setFocus();
	setFocusPolicy(Qt::ClickFocus);
	setMouseTracking(true);

	activeDomain = 0;

	viewportWidth = 0.0;
	viewportHeight = 0.0;

//	xDomain = 0.0;
//	yDomain = 0.0;

//	clicking = false;

//	viewMode = DisplayMode;

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

//	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClearColor(0.1, 0.2, 0.3, 1.0);
	glDisable(GL_DEPTH_TEST);
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

//	if (clicking == false && activeDomain)
//		activeDomain->Zoom(event->delta());

	if (activeDomain)
		activeDomain->MouseWheel(event);

//	updateGL();
}


/**
 * @brief Event fired when a mouse button is clicked
 * @param event
 */
void OpenGLPanel::mousePressEvent(QMouseEvent *event)
{
//	clicking = true;
//	pushedButton = event->button();

//	oldx = event->x();
//	oldy = event->y();
//	mouseMoved = false;

//	if (viewMode == CircleSubdomainMode || viewMode == RectangleSubdomainMode)
//	{
		if (activeDomain)
			activeDomain->MouseClick(event);
//	}
}


/**
 * @brief Event fired when the mouse is moved over the panel
 * @param event
 */
void OpenGLPanel::mouseMoveEvent(QMouseEvent *event)
{
//	mouseMoved = true;
//	newx = event->x();
//	newy = event->y();
//	dx = newx-oldx;
//	dy = newy-oldy;

//	emit mouseCoordinates(newx, newy);

//	if (clicking)
//	{
//		if (viewMode == DisplayMode)
//		{
//			if (activeDomain)
//				activeDomain->Pan(dx, dy);
//		}
//		else if (viewMode == CircleSubdomainMode || viewMode == RectangleSubdomainMode)
//		{
			if (activeDomain)
				activeDomain->MouseMove(event);
//		}

//		updateGL();
//	}

//	oldx = newx;
//	oldy = newy;
}


/**
 * @brief Event fired when a mouse button is released
 * @param event
 */
void OpenGLPanel::mouseReleaseEvent(QMouseEvent *event)
{
//	clicking = false;

//	if (!mouseMoved)
//	{
//		if (viewMode == DisplayMode)
//		{
//			float x, y;
//			if (currentCam)
//				currentCam->GetUnprojectedPoint(event->x(), event->y(), &x, &y);
//		}
//	}

//	if (viewMode == CircleSubdomainMode)
//	{
		if (activeDomain)
			activeDomain->MouseRelease(event);
//			activeDomain->SetCircleToolFinished();
//		viewMode = DisplayMode;
//	}

//	mouseMoved = false;

//	updateGL();
}


/**
 * @brief Sets the visible Domain
 *
 * Sets the visible Domain. Does not destroy or keep track of any previously visible Domain.
 *
 * @param The Domain to make visible
 */
void OpenGLPanel::SetActiveDomain(Domain *newDomain)
{
	/* Disconnect the old domain */
	if (activeDomain)
	{
		disconnect(activeDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
//		disconnect(this, SIGNAL(mouseCoordinates(float,float)), activeDomain, SLOT(setMouseCoordinates(float,float)));
	}

	/* Set up connections with the new one */
	activeDomain = newDomain;
	activeDomain->SetWindowSize(viewportWidth, viewportHeight);
	connect(activeDomain, SIGNAL(updateGL()), this, SLOT(updateGL()));
//	connect(this, SIGNAL(mouseCoordinates(float,float)), activeDomain, SLOT(setMouseCoordinates(float,float)));
}


///**
// * @brief Sets the current view mode to Display Mode
// */
//void OpenGLPanel::enterDisplayMode()
//{
//	viewMode = DisplayMode;
//}


///**
// * @brief Sets the current view mode to Circle Subdomain Mode
// */
//void OpenGLPanel::enterCircleSubdomainMode()
//{
//	viewMode = CircleSubdomainMode;
//}


//void OpenGLPanel::enterRectangleSubdomainMode()
//{
//	viewMode = RectangleSubdomainMode;
//}
