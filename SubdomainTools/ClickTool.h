#ifndef CLICKTOOL_H
#define CLICKTOOL_H

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "SubdomainTools/SelectionTool.h"

class ClickTool : public SelectionTool
{
		Q_OBJECT
	public:
		ClickTool();
		~ClickTool();

		void	Draw();
		void	SetCamera(GLCamera* cam);
		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetViewportSize(float w, float h);

		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	KeyPress(QKeyEvent *event);

		void	UseTool();

		std::vector<Node*>	GetSelectedNodes();
		std::vector<Element*>	GetSelectedElements();

	private:

		TerrainLayer*	terrain;
		GLCamera*	camera;

		/* Helper Functions */
		void	InitializeGL();
		void	UpdateCoordinates();
		void	UpdateGL();
		void	FindElement();

		/* OpenGL Stuff */
		bool		glLoaded;
		bool		visible;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;
		SolidShader*	pointShader;
		GLfloat		glPoint[4];
		GLuint		glIndex;

		/* Mouse State */
		bool	mousePressed;
		int	xPixel;
		int	yPixel;
		float	xGL;
		float	yGL;

		/* Selected Nodes/Elements */
		std::vector<Node*>	selectedNodes;
		std::vector<Element*>	selectedElements;

		/* Viewport Attributes */
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */
};

#endif // CLICKTOOL_H
