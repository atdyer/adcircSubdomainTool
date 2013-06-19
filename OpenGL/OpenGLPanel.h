#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "glew.h"
#include <QGLWidget>


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

	protected:

		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

	signals:

		void	emitMessage(const char*);
};

#endif // OPENGLPANEL_H
