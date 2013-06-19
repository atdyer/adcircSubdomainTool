#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "glew.h"
#include <QGLWidget>

#include <OpenGL/GLCamera.h>


/**
 * @brief This is a custom widget that is used specifically for drawing Layer objects
 *
 * Use this custom widget in conjuntion with a LayerManager to draw multiple Layers
 *
 */
class OpenGLPanel : public QGLWidget
{
		Q_OBJECT
	public:
		explicit	OpenGLPanel(QWidget *parent = 0);

		void	SetCamera(GLCamera* newCam);

	protected:

		GLCamera*	camera;

		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

	signals:

		void	emitMessage(const QString &);
};

#endif // OPENGLPANEL_H
