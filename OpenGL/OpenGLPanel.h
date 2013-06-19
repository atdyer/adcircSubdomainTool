#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "glew.h"
#include <QGLWidget>
#include <Layers/LayerManager.h>


// Forward declaration of LayerManger class
//class LayerManager;


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
		void		SetLayerManager(LayerManager* newManager);

	protected:

		LayerManager*	layerManager;	/**< The LayerManager that will do all of the drawing */
		GLCamera*	currentCam;	/**< The GLCamera currently being used in the LayerManager */

		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

	public slots:

		void	updateCurrentCamera();

	signals:

		void	emitMessage(QString);
};

#endif // OPENGLPANEL_H
