#ifndef POLYGONTOOL_H
#define POLYGONTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "SubdomainTools/SelectionTool.h"

class PolygonTool : public SelectionTool
{
		Q_OBJECT
	public:
		PolygonTool();
		~PolygonTool();

		void	Draw();
		void	SetCamera(GLCamera *cam);
		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetViewportSize(float w, float h);
		void	SetSelectionMode(SelectionType newMode);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	UseTool();

		void	MouseClick(int x, int y);
		void	MouseMove(int x, int y);
		void	MouseRelease(int x, int y);

		void	StartUsingTool();
		void	FinishDrawingTool();

		std::vector<Element*>	GetSelectedElements();

//	signals:

//		void	ToolFinishedDrawing();
//		void	ToolFinishedSearching();

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
		SolidShader*	lineShader;		/**< The shader used to draw the selection tool */
		void		InitializeGL();
		void		UpdateVertexBuffer();
		void		UpdateIndexBuffer();
		void		UpdateMouseVertex();

		/* Mouse Attributes */
		float	mouseX;
		float	mouseY;
		bool	mousePressed;
		bool	mouseMoved;
		void	AddPoint(float x, float y);
		bool	CheckForDoubleClick(float x, float y);

		/* Selection Mode */
		SelectionType	selectionMode;	/**< The current selection mode */

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;
		std::vector<Element*>	selectedElements;

		/* Polygon Attributes */
		int			pointCount;
		std::vector<Point>	pointsList;

		/* Viewport Attributes */
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

};

#endif // POLYGONTOOL_H
