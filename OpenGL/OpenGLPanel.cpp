#include "OpenGLPanel.h"
#include <iostream>

OpenGLPanel::OpenGLPanel(QWidget *parent) :
	QGLWidget(parent)
{
}

void OpenGLPanel::initializeGL()
{
	glewExperimental = true;
	GLenum err = glewInit();

	if (err != GLEW_OK)
	{
		std::cout << "Error: GLEW unable to load OpenGL Extensions" << std::endl;
		exit(EXIT_FAILURE);
	} else {
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


void OpenGLPanel::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}


void OpenGLPanel::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
