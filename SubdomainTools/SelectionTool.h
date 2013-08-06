#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"

class SelectionTool : public QObject
{
		Q_OBJECT
	public:
		SelectionTool();

		virtual void	Draw() = 0;
		virtual void	SetCamera(GLCamera *cam) = 0;
		virtual void	SetTerrainLayer(TerrainLayer *layer) = 0;
		virtual void	SetViewportSize(float w, float h) = 0;
		virtual void	SetSelectionMode(SelectionType newMode) = 0;

		virtual void	MouseClick(QMouseEvent *event) = 0;
		virtual void	MouseMove(QMouseEvent *event) = 0;
		virtual void	MouseRelease(QMouseEvent *event) = 0;
		virtual void	MouseWheel(QWheelEvent *event) = 0;
		virtual void	KeyPress(QKeyEvent *event) = 0;

		virtual void	UseTool() = 0;
		void		UseTool(SelectionType newMode);

		virtual std::vector<Element*>	GetSelectedElements() = 0;

	signals:

		void	Message(QString);
		void	Instructions(QString);
		void	ToolFinishedDrawing();
		void	ToolFinishedSearching();

//	protected:

//		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
//		GLCamera*	camera;		/**< The GLCamera that is used to draw the TerrainLayer */

//		/* Selection Mode */
//		SelectionType	selectionMode;	/**< The current selection mode */

//		/* Viewport Attributes */
//		float	w;	/**< The viewport width */
//		float	h;	/**< The viewport height */
//		float	l;	/**< The left side of the viewport */
//		float	r;	/**< The right side of the viewport */
//		float	b;	/**< The bottom of the viewport */
//		float	t;	/**< The top of the viewport */

//		/* OpenGL Stuff */
//		bool		glLoaded;		/**< Flag that shows if the VAO/VBO/IBO have been created */
//		bool		visible;		/**< Flag that shows if the tool is currently visible */
//		GLuint		VAOId;			/**< The vertex array object ID */
//		GLuint		VBOId;			/**< The vertex buffer object ID */
//		GLuint		IBOId;			/**< The index buffer object ID */

//		/* Virtual Functions */
//		void	UpdateCamera() = 0;
//		void	UpdateTerrain() = 0;
//		void	UpdateViewport() = 0;
};

#endif // SELECTIONTOOL_H
