#ifndef SELECTIONLAYER_H
#define SELECTIONLAYER_H

#include "adcData.h"
#include "Layers/Layer.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/GLCamera.h"

#include <vector>


/**
 * @brief The SelectionLayer class keeps track of and draws Nodes and Elements that have been
 * selected by the user.
 *
 * The SelectionLayer class keeps track of and draws Nodes and Elements that have been
 * selected by the user. It keeps track of and has the ability to undo previous actions.
 * It is unique from other Layer objects in that it is responsible for allocating/deallocating
 * memory for its own GLShader objects.
 *
 */
class SelectionLayer : public Layer
{
		Q_OBJECT
	public:

		// Constructor/Destructor
		SelectionLayer();
		~SelectionLayer();

		// Virtual Methods to override
		virtual void	Draw();
		virtual void	LoadDataToGPU();

		// Setter Methods
		void	SetCamera(GLCamera *cam);


	protected:

		// Selected Nodes and Elements
		std::vector<Node*>	selectedNodes;
		std::vector<Element*>	selectedElements;

		// OpenGL Variables
		GLCamera*	camera;		/**< The camera used to draw selections */
		GLuint		VAOId;		/**< The vertex array object ID in the OpenGL context */
		GLuint		VBOId;		/**< The vertex buffer object ID in the OpenGL context */
		GLuint		IBOId;		/**< The index buffer object ID in the OpenGL context */
		SolidShader*	pointShader;	/**< The shader used to draw Nodes */
		SolidShader*	outlineShader;	/**< The shader used to draw Element outlines */
		SolidShader*	fillShader;	/**< The shader used to draw Element fill */

		// Flags
		bool	undoable;	/**< Flag that shows if an undo operation can be performed */
		bool	glLoaded;	/**< Flag that shows if data has been successfully sent to the GPU */

		void	UpdateDataOnGPU();



	signals:

		void	undoAvailable();
		void	undoUnavailable();
		void	numNodesSelected(int);

	public slots:

		void	SelectNode(Node* node);
		void	SelectNodes(std::vector<Node*> nodes);
		void	undo();


};

#endif // SELECTIONLAYER_H
