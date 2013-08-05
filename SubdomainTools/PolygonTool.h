#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"

class PolygonTool : public QObject
{
		Q_OBJECT
	public:
		PolygonTool();
		~PolygonTool();

		void	Draw();
		void	SetCamera(GLCamera *cam);
		void	SetViewportSize(float w, float h);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		std::vector<Element*>	GetSelectedElements();

	public slots:

		void	StartUsingTool();

	signals:

		void	ToolFinishedDrawing();
		void	ToolFinishedSearching();

	private:

		struct Point
		{
				float x;
				float y;
				Point() : x(0.0), y(0.0) {}
				Point(float a, float b) : x(a), y(b) {}
		};

		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is used to draw the TerrainLayer */

		/* OpenGL Stuff */
		bool		glLoaded;		/**< Flag that shows if the VAO/VBO/IBO have been created */
		bool		visible;		/**< Flag that shows if the tool is currently visible */
		GLuint		VAOId;			/**< The vertex array object ID */
		GLuint		VBOId;			/**< The vertex buffer object ID */
		GLuint		IBOId;			/**< The index buffer object ID */
		SolidShader*	fillShader;		/**< The shader used to draw the selection tool */

		/* Selection Mode */
		SelectionType	selectionMode;	/**< The current selection mode */

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;
		std::vector<Element*>	selectedElements;

		/* Polygon Attributes */
		std::vector<Point>	pointsList;

};

#endif // POLYGONTOOL_H
