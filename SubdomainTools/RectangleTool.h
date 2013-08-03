#ifndef RECTANGLETOOL_H
#define RECTANGLETOOL_H

#include <QObject>
#include <string>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"


class RectangleTool : public QObject
{
		Q_OBJECT
	public:
		RectangleTool();
		~RectangleTool();

		void	Draw();
		void	InitializeGL();
		void	UpdateGL();
		void	SetCamera(GLCamera* cam);
		void	SetViewportSize(float w, float h);
		void	SetFirstCorner(int newX, int newY);
		void	SetSecondCorner(int newX, int newY);
		void	RectangleFinished();
		void	CalculateVertexPoints();

		std::vector<Element*>	GetSelectedElements();

	protected:

		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is used to draw the TerrainLayer */

		/* OpenGL Stuff */
		bool		glLoaded;		/**< Flag that shows if the VAO/VBO/IBO have been created */
		bool		visible;		/**< Flag that shows if the tool is currently visible */
		GLuint		VAOId;			/**< The vertex array object ID */
		GLuint		VBOId;			/**< The vertex buffer object ID */
		GLuint		IBOId;			/**< The index buffer object ID */
		SolidShader*	fillShader;		/**< The shader used to draw the selection tool */
		GLfloat		vertexPoints[12][4];	/**< The actual vertices used to draw the tool */
		GLuint		indexArray[8][3];	/**< The indices that make up the triangles */
		size_t		vertexBufferSize;	/**< The size of the vertex data in bytes */

		/* Selection Mode */
		SelectionType	selectionMode;	/**< The current selection mode */

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;
		std::vector<Element*>	selectedElements;

		/* Rectangle Attributes */
		float	firstCornerPixel[2];	/**< The coordinates of the first corner of the rectangle, in pixels */
		float	secondCornerPixel[2];	/**< The coordinates of the second corner of the rectangle, in pixels */

		float	firstCornerNormal[2];	/**< The coordinates of the first corner of the rectangle, in normalized OpenGL space */
		float	secondCornerNormal[2];	/**< The coordinates of the second corner of the rectangle, in normalized OpenGL space */

		float	firstCornerDomain[2];	/**< The coordinates of the first corner of the rectangle, in the original coordinate system of TerrainLayer */
		float	secondCornerDomain[2];	/**< The coordinates of the second corner of the rectangle, in the original coordinate system of TerrainLayer */

		/* Viewport Attributes */
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

	signals:

		void	RectangleStatsSet(float w, float h);
		void	RectangleStatsFinished();
		void	ToolFinishedDrawing();

	public slots:

		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetSelectionMode(SelectionType newMode);
};

#endif // RECTANGLETOOL_H
