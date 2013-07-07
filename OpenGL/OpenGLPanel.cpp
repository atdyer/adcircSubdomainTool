#include "OpenGLPanel.h"


OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
	setFocus();
	setFocusPolicy(Qt::ClickFocus);

	layerManager = 0;
	currentCam = 0;

	viewMode = DisplayMode;
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
void OpenGLPanel::SetLayerManager(LayerManager *newManager)
{
	if (newManager)
	{
		layerManager = newManager;
		updateCurrentCamera();
	}
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

	glClearColor(0.1, 0.2, 0.3, 1.0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glDepthFunc(GL_LESS);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPointSize(10);
	glEnable(GL_POINT_SMOOTH);
}


/**
 * @brief Called with the size of the OpenGL context changes
 * @param w The new width
 * @param h The new height
 */
void OpenGLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	if (currentCam)
		currentCam->SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -100.0, 100.0);
}


/**
 * @brief Called every time the OpenGL context needs to update the display
 */
void OpenGLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (layerManager)
		layerManager->DrawVisibleLayers();
	else
		DEBUG("No Layer Manager Assigned");
}


void OpenGLPanel::wheelEvent(QWheelEvent *event)
{
	if (viewMode == DisplayMode)
	{
		if (currentCam)
			currentCam->Zoom(event->delta());
	}
	updateGL();
}


void OpenGLPanel::mousePressEvent(QMouseEvent *event)
{
	pushedButton = event->button();
	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;
}


void OpenGLPanel::mouseMoveEvent(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	if (viewMode == DisplayMode)
	{
		if (currentCam)
			currentCam->Pan(dx, dy);
	}

	oldx = newx;
	oldy = newy;

	updateGL();
}


void OpenGLPanel::mouseReleaseEvent(QMouseEvent *event)
{
	if (!mouseMoved)
	{
		if (viewMode == DisplayMode)
		{
			float x, y;
			if (currentCam)
				currentCam->GetUnprojectedPoint(event->x(), event->y(), &x, &y);
		}
	}
	mouseMoved = false;
}


void OpenGLPanel::updateCurrentCamera()
{
	if (layerManager)
		currentCam = layerManager->GetCurrentCamera();
}


void OpenGLPanel::enterDisplayMode()
{

}


void OpenGLPanel::enterCircleSubdomainMode()
{

}
