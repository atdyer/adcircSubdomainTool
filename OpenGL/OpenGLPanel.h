#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "glew.h"
#include <QGLWidget>
#include <QWheelEvent>

#include <Layers/LayerManager.h>
#include <SubdomainTools/CircleTool.h>

/**
 * @brief The Mode enum contains the available states for the OpenGLPanel. The
 * state defines what happens when the user interacts with the panel through
 * clicks and key presses.
 */
enum Mode {DisplayMode, CircleSubdomainMode};

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

		CircleTool	circleTool;	/**< Tool for selecting nodes by drawing a circle */

		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

		// Mouse and key events
		void	wheelEvent(QWheelEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);
		void	mouseReleaseEvent(QMouseEvent *event);

	private:

		enum	Mode viewMode;
		float	xDomain, yDomain;
		int	oldx, oldy, newx, newy, dx, dy;
		bool	clicking, mouseMoved;
		enum	Qt::MouseButton pushedButton;

	public slots:

		void	updateCurrentCamera();
		void	enterDisplayMode();
		void	enterCircleSubdomainMode();

	signals:

		void	emitMessage(QString);
		void	mouseX(float);
		void	mouseY(float);
		void	circleToolStatsSet(float, float, float);
		void	circleToolStatsFinished();
};

#endif // OPENGLPANEL_H
