#ifndef SELECTIONLAYER_H
#define SELECTIONLAYER_H

#include <vector>
#include <stack>

#include "Layers/Layer.h"
#include "Layers/TerrainLayer.h"

#include "OpenGL/GLCamera.h"

#include "Actions/Action.h"


/**
 * @brief A virtual class that contains all of the generic functions needed for
 * a selection layer.
 *
 * A virtual class that contains all of the generic functions needed for a selection layer.
 * Subclasses of SelectionLayer should contain all of the tools necessary for performing
 * a specific subset of selection functionalities. For example, when creating a subdomain,
 * the user only needs to be able to select elements, so the selection layer that is
 * used for creating subdomains should only contain tools that are needed for selecting
 * elements.
 *
 * Because selections will always come from mesh data, every selection layer will have
 * a TerrainLayer that it gets that mesh data from.
 *
 */
class SelectionLayer : public Layer
{
		Q_OBJECT
	public:

		SelectionLayer();
		~SelectionLayer();

		virtual void	Draw() = 0;
		virtual void	LoadDataToGPU() = 0;

		virtual unsigned int	GetNumNodesSelected();
		virtual unsigned int	GetNumElementsSelected();

		virtual void	SetCamera(GLCamera *newCamera) = 0;
		virtual void	SetTerrainLayer(TerrainLayer* newLayer) = 0;
		virtual void	UseTool(int toolID) = 0;

		virtual void	MouseClick(int x, int y) = 0;
		virtual void	MouseMove(int x, int y) = 0;
		virtual void	MouseRelease(int x, int y) = 0;
		virtual void	WindowSizeChanged(float w, float h) = 0;

		virtual void	Undo();
		virtual void	Redo();

	protected:

		/* Layers */
		TerrainLayer*	terrainLayer;	/**< The Terrain Layer that the selections are taken from */

		/* Undo and Redo Stacks */
		std::stack<Action*, std::vector<Action*> >	undoStack;
		std::stack<Action*, std::vector<Action*> >	redoStack;

		/* OpenGL Variables */
		bool		glLoaded;
		GLCamera*	camera;
		GLuint		VAOId;
		GLuint		VBOId;
		GLuint		IBOId;

		/* Flags */
		bool	mousePressed;

		/* Helper Functions */
		void	ClearUndoStack();
		void	ClearRedoStack();

	signals:

		void	UndoAvailable(bool);
		void	RedoAvailable(bool);
		void	Refreshed();
		void	NumNodesSelected(int);
		void	NumElementsSelected(int);
};


/** OLD CODE STARTS HERE */

//#include "adcData.h"
//#include "Layers/Layer.h"
//#include "Layers/Actions/Action.h"
//#include "Layers/Actions/NodeAction.h"
//#include "Layers/Actions/ElementAction.h"
//#include "OpenGL/Shaders/SolidShader.h"
//#include "OpenGL/GLCamera.h"

//#include <map>
//#include <stack>
//#include <vector>


///**
// * @brief The SelectionLayer class keeps track of and draws Nodes and Elements that have been
// * selected by the user.
// *
// * The SelectionLayer class keeps track of and draws Nodes and Elements that have been
// * selected by the user. It keeps track of and has the ability to undo previous actions.
// *
// * The lists of selected Node and Element objects are implemented as std::map for performance
// * reasons. These lists need to have unique entries, and std::map will not add duplicates.
// *
// * Undo/Redo capabilities are implemented using two action stacks. When the user performs an action
// * such as selected a Node or Element, a subclass of the Action class, such as NodeAction or
// * ElementAction is created and pushed onto the undo stack. When the user clicks the undo button,
// * the top Action is popped off of the undo stack, its Action::UndoAction() function is called,
// * and it is pushed onto the redo stack. Similarly for the redo stack, the top Action is popped off,
// * its Action::RedoAction() function is called, and it is pushed onto the undo stack.
// *
// */
//class SelectionLayer : public Layer
//{
//		Q_OBJECT
//	public:

//		friend class NodeAction;
//		friend class ElementAction;

//		// Constructor/Destructor
//		SelectionLayer();
//		~SelectionLayer();

//		// Virtual Methods to override
//		virtual void	Draw();
//		virtual void	LoadDataToGPU();

//		// Setter Methods
//		virtual void	SetCamera(GLCamera *cam);

//		// Getter Methods
//		unsigned int	GetNumNodesSelected();
//		unsigned int	GetNumElementsSelected();


//	protected:

//		// Selected Nodes and Elements
//		std::map<unsigned int, Node*>		selectedNodes;		/**< The map of all selected Nodes - <Node #, Pointer to Node> */
//		std::map<unsigned int, Element*>	selectedElements;	/**< The map of all selected Elements - <Element #, Pointer to Element> */
//		std::map<unsigned int, unsigned int>	indexingMap;		/**< The map used for getting Node indexing within the selection - <Node #, Index of Node in Selected List> */

//		// Undo and Redo stacks
//		std::stack<Action*, std::vector<Action*> >	undoStack;	/**< The stack of all undo actions */
//		std::stack<Action*, std::vector<Action*> >	redoStack;	/**< The stack of all redo actions */

//		// OpenGL Variables
//		GLCamera*	camera;		/**< The camera used to draw selections */
//		GLuint		VAOId;		/**< The vertex array object ID in the OpenGL context */
//		GLuint		VBOId;		/**< The vertex buffer object ID in the OpenGL context */
//		GLuint		IBOId;		/**< The index buffer object ID in the OpenGL context */
//		SolidShader*	pointShader;	/**< The shader used to draw Nodes */
//		SolidShader*	outlineShader;	/**< The shader used to draw Element outlines */
//		SolidShader*	fillShader;	/**< The shader used to draw Element fill */

//		// Flags
//		bool	glLoaded;	/**< Flag that shows if the VAO/VBO/IBO have been created */

//		// Helper Functions
//		void	InitializeGL();
//		void	UpdateVertexBuffer();
//		void	UpdateIndexBuffer();
//		void	UpdateNodeIndexing();

//		// Selection Functions used by Actions
//		void	SelectNodes(std::map<unsigned int, Node*> nodes);
//		void	DeselectNodes(std::map<unsigned int, Node*> nodes);
//		void	SelectElements(std::map<unsigned int, Element*> elements);
//		void	DeselectElements(std::map<unsigned int, Element*> elements);



//	signals:

//		void	undoAvailable(bool);		/**< Triggered after the user performs an undoable action */
//		void	redoAvailable(bool);		/**< Triggered after the user performs an undo operation that can be redone */
//		void	numNodesSelected(int);		/**< Triggered when the number of selected Nodes changes */
//		void	numElementsSelected(int);	/**< Triggered when the number of selected Elements changes */
//		void	refreshed();			/**< Triggered when data has been loaded to the OpenGL context and is ready to be drawn */

//	public slots:

//		void	SelectNode(Node* node);
//		void	SelectNodes(std::vector<Node*> nodes);
//		void	SelectElements(std::vector<Element*> elements);
//		void	undo();
//		void	redo();


//};

#endif // SELECTIONLAYER_H
