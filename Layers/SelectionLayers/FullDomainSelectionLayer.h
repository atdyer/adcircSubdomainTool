#ifndef FULLDOMAINSELECTIONLAYER_H
#define FULLDOMAINSELECTIONLAYER_H

#include <vector>
#include <stack>
#include <algorithm>

#include "Layers/SelectionLayer.h"

#include "NewProjectModel/Files/Fort14_new.h"

#include "SubdomainTools/SelectionTool.h"
#include "SubdomainTools/ClickTool.h"
#include "SubdomainTools/CircleTool.h"
#include "SubdomainTools/RectangleTool.h"
#include "SubdomainTools/PolygonTool.h"
#include "SubdomainTools/BoundaryFinder.h"

class FullDomainSelectionLayer : public SelectionLayer
{
		Q_OBJECT
	public:
		FullDomainSelectionLayer(Fort14_new *fort14, QObject *parent=0);
		~FullDomainSelectionLayer();

		virtual void	Draw();
		virtual void	LoadDataToGPU();
		virtual void	SetData(QString fileLocation);
		virtual bool	DataLoaded();

		virtual unsigned int	GetNumNodesSelected();
		virtual unsigned int	GetNumElementsSelected();

		virtual void	SetCamera(GLCamera *newCamera);
		virtual void	UseTool(ToolType tool, SelectionType selection);

		virtual void	MouseClick(QMouseEvent *event);
		virtual void	MouseMove(QMouseEvent *event);
		virtual void	MouseRelease(QMouseEvent *event);
		virtual void	MouseWheel(QWheelEvent *event);
		virtual void	KeyPress(QKeyEvent *event);
		virtual void	WindowSizeChanged(float w, float h);

		virtual void	Undo();
		virtual void	Redo();

	private:

		Fort14_new	*fort14;

		/* Selection Tools */
		ToolType	activeToolType;
		SelectionTool*	activeTool;
		ClickTool*	clickTool;
		CircleTool*	circleTool;
		RectangleTool*	rectangleTool;
		PolygonTool*	polygonTool;
		BoundaryFinder*	boundaryFinder;

		/* Selected Elements */
		ElementState*	selectedState;

		/* Boundary Nodes */
		std::vector<unsigned int>	outerBoundaryNodes;
		std::vector<unsigned int>	innerBoundaryNodes;

		/* Undo and Redo Stacks */
		std::stack<ElementState*, std::vector<ElementState*> >	undoStack;
		std::stack<ElementState*, std::vector<ElementState*> >	redoStack;

		/* Shaders */
		SolidShader*	outlineShader;
		SolidShader*	fillShader;
		SolidShader*	innerBoundaryShader;
		SolidShader*	outerBoundaryShader;

		/* Initialization/Deallocation Functions */
		void	AttachToFort14();
		void	ClearUndoStack();
		void	ClearRedoStack();
		void	InitializeGL();

		/* Tool Initialization Functions */
		void	CreateClickTool();
		void	CreateCircleTool();
		void	CreateRectangleTool();
		void	CreatePolygonTool();

		/* State Functions */
		void	UseNewState(ElementState* newState);
		void	UseState(ElementState* state);
		void	GetSelectionFromActiveTool();

	private slots:

		void	UseVBOId(GLuint newVBO);
		void	GetSelectionFromTool();
};

#endif // FULLDOMAINSELECTIONLAYER_H
