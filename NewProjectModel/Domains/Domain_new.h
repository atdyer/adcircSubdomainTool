#ifndef DOMAIN_NEW_H
#define DOMAIN_NEW_H

#include <QObject>
#include <QProgressBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "NewProjectModel/Files/ProjectFile_new.h"
#include "NewProjectModel/Files/Fort14_new.h"

#include "OpenGL/GLCamera.h"

#include "Layers/SelectionLayer.h"

class Domain_new : public QObject
{
		Q_OBJECT
	public:
		Domain_new(ProjectFile_new *projectFile, QObject *parent=0);
		~Domain_new();

		void		Draw();
		virtual bool	IsFullDomain() = 0;

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	SetCamera(GLCamera *newCam);
		void	SetProgressBar(QProgressBar* newBar);
		void	SetWindowSize(float w, float h);

	protected:

		GLCamera*		camera;
		Fort14_new*		fort14;
		QProgressBar*		progressBar;
		ProjectFile_new*	projectFile;
		SelectionLayer*		selectionLayer;

		/* Mouse Clicking and Moving Stuff */
		ActionType	currentMode;	/**< The current mode used to determine where actions are sent */
		int		oldx;	/**< The old x-coordinate of the mouse */
		int		oldy;	/**< The old y-coordinate of the mouse */
		int		newx;	/**< The new x-coordinate of the mouse */
		int		newy;	/**< The new y-coordinate of the mouse */
		int		dx;	/**< The change in x-coordinates between mouse events */
		int		dy;	/**< The change in y-coordinates between mouse events */
		Qt::MouseButton	pushedButton;	/**< The mouse button being pushed */
		bool		clicking;	/**< Flag that shows if a mouse button is being held down */
		bool		mouseMoved;	/**< Flag that shows if the mouse has moved since the last event */


		void		Zoom(float zoomAmount);
		void		Pan(float dx, float dy);
		void		CalculateMouseCoordinates();

		
	signals:

		void	updateGL();
		void	setCursor(const QCursor &);
		void	mouseX(float);
		void	mouseY(float);
		
	public slots:

		void	SetTerrainSolidOutline(QColor newColor);
		void	SetTerrainSolidFill(QColor newColor);
		void	SetTerrainGradientOutline(QGradientStops newStops);
		void	SetTerrainGradientFill(QGradientStops newStops);
		
};

#endif // DOMAIN_NEW_H
