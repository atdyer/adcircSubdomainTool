#ifndef OPENGLPANEL_H
#define OPENGLPANEL_H

#include "OpenGL/glew.h"
#include <QGLWidget>
#include <QWheelEvent>

#include "Layers/SelectionLayer.h"
#include "SubdomainTools/CircleTool.h"
#include "Domains/Domain.h"

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
		void		SetActiveDomain(Domain* newDomain);

	protected:

		Domain*		activeDomain;	/**< The Domain currently being displayed */

		void		initializeGL();
		void		resizeGL(int w, int h);
		void		paintGL();

		// Mouse and key events
		void	wheelEvent(QWheelEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);
		void	mouseReleaseEvent(QMouseEvent *event);

	private:

		int	viewportWidth, viewportHeight;
		enum	Mode viewMode;
		float	xDomain, yDomain;
		int	oldx, oldy, newx, newy, dx, dy;
		bool	clicking, mouseMoved;
		enum	Qt::MouseButton pushedButton;

	public slots:

		void	enterDisplayMode();
		void	enterCircleSubdomainMode();

	signals:

		void	emitMessage(QString);
		void	mouseCoordinates(float, float);
		void	circleToolStatsSet(float, float, float);
		void	circleToolStatsFinished();
		void	numNodesSelected(int);
		void	numElementsSelected(int);
		void	undoAvailable(bool);
		void	undo();
		void	redoAvailable(bool);
		void	redo();
};

#endif // OPENGLPANEL_H
