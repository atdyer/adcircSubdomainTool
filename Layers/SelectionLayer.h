#ifndef SELECTIONLAYER_H
#define SELECTIONLAYER_H

#include "adcData.h"
#include "Layers/Layer.h"
#include "Layers/Actions/Action.h"
#include "Layers/Actions/NodeAction.h"
#include "Layers/Actions/ElementAction.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/GLCamera.h"

#include <map>
#include <stack>
#include <vector>


/**
 * @brief The SelectionLayer class keeps track of and draws Nodes and Elements that have been
 * selected by the user.
 *
 * The SelectionLayer class keeps track of and draws Nodes and Elements that have been
 * selected by the user. It keeps track of and has the ability to undo previous actions.
 *
 * The lists of selected Node and Element objects are implemented as std::map for performance
 * reasons. These lists need to have unique entries, and std::map will not add duplicates.
 *
 * Undo/Redo capabilities are implemented using two action stacks. When the user performs an action
 * such as selected a Node or Element, a subclass of the Action class, such as NodeAction or
 * ElementAction is created and pushed onto the undo stack. When the user clicks the undo button,
 * the top Action is popped off of the undo stack, its Action::UndoAction() function is called,
 * and it is pushed onto the redo stack. Similarly for the redo stack, the top Action is popped off,
 * its Action::RedoAction() function is called, and it is pushed onto the undo stack.
 *
 */
class SelectionLayer : public Layer
{
		Q_OBJECT
	public:

		friend class NodeAction;
		friend class ElementAction;

		// Constructor/Destructor
		SelectionLayer();
		~SelectionLayer();

		// Virtual Methods to override
		virtual void	Draw();
		virtual void	LoadDataToGPU();

		// Setter Methods
		virtual void	SetCamera(GLCamera *cam);

		// Getter Methods
		unsigned int	GetNumNodesSelected();
		unsigned int	GetNumElementsSelected();


	protected:

		// Selected Nodes and Elements
		std::map<unsigned int, Node*>		selectedNodes;		/**< The map of all selected Nodes - <Node #, Pointer to Node> */
		std::map<unsigned int, Element*>	selectedElements;	/**< The map of all selected Elements - <Element #, Pointer to Element> */

		// Undo and Redo stacks
		std::stack<Action*, std::vector<Action*> >	undoStack;	/**< The stack of all undo actions */
		std::stack<Action*, std::vector<Action*> >	redoStack;	/**< The stack of all redo actions */

		// OpenGL Variables
		GLCamera*	camera;		/**< The camera used to draw selections */
		GLuint		VAOId;		/**< The vertex array object ID in the OpenGL context */
		GLuint		VBOId;		/**< The vertex buffer object ID in the OpenGL context */
		GLuint		IBOId;		/**< The index buffer object ID in the OpenGL context */
		SolidShader*	pointShader;	/**< The shader used to draw Nodes */
		SolidShader*	outlineShader;	/**< The shader used to draw Element outlines */
		SolidShader*	fillShader;	/**< The shader used to draw Element fill */

		// Flags
		bool	glLoaded;	/**< Flag that shows if the VAO/VBO/IBO have been created */

		// Helper Functions
		void	InitializeGL();
		void	UpdateVertexBuffer();

		// Selection Functions used by Actions
		void	SelectNodes(std::map<unsigned int, Node*> nodes);
		void	DeselectNodes(std::map<unsigned int, Node*> nodes);



	signals:

		void	undoAvailable(bool);		/**< Triggered after the user performs an undoable action */
		void	redoAvailable(bool);		/**< Triggered after the user performs an undo operation that can be redone */
		void	numNodesSelected(int);		/**< Triggered when the number of selected Nodes changes */
		void	numElementsSelected(int);	/**< Triggered when the number of selected Elements changes */
		void	refreshed();			/**< Triggered when data has been loaded to the OpenGL context and is ready to be drawn */

	public slots:

		void	SelectNode(Node* node);
		void	SelectNodes(std::vector<Node*> nodes);
		void	undo();
		void	redo();


};

#endif // SELECTIONLAYER_H
