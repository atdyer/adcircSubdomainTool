#ifndef CREATIONSELECTIONLAYER_H
#define CREATIONSELECTIONLAYER_H

#include <vector>
#include <stack>
#include <algorithm>

#include "Layers/Layer.h"
#include "Layers/TerrainLayer.h"

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/SolidShader.h"

#include "SubdomainTools/CircleTool.h"

#include <QObject>

#define AVAILABLETOOLS 1
#define CIRCLETOOLINDEX 1


/**
 * @brief A Layer class used for the creation of subdomains. Specializes in selecting
 * elements from large domains.
 *
 * This is a selection layer that is used only for selecting the elements needed
 * to create a subdomain. It is unable to select nodes separately from elements,
 * as this functionality is not needed when creating subdomains. A number of tools
 * are available for selecting (and deselecting) elements:
 * - CircleTool - select elements inside of a circle
 * - EllipseTool - select elements inside of an ellipse
 * - PolyLineTool - select elements inside of an arbitrary shape
 * - RectangleTool - select elements inside of a rectangle
 * - LineTool - select elements underneath a line
 *
 *
 * I decided to use the Vertex Buffer Object from the TerrainLayer for drawing
 * selected elements. This gives us a number of benefits over storing selected
 * nodes locally:
 * - Don't have to recalculate indices in local node list after every action
 * - Less local data means undo/redo stack can save state at each interaction
 *   without having to worry about memory. This means we don't have to worry
 *   about selecting only unique elements at each interaction.
 * - Space is saved on the GPU
 *
 * For the undo/redo stack, because the typical subdomain size is relatively
 * small (compared to memory available), we keep track of the complete state
 * of selected elements after each interaction. Also, because only one type
 * of action is possible (selecting/deselecting elements), we don't need
 * to use the Action class. We can just use two stacks that contain previous
 * and future selection states.
 *
 */
class CreationSelectionLayer : public Layer
{
		Q_OBJECT
	public:
		CreationSelectionLayer();
		~CreationSelectionLayer();

		virtual void	Draw();
		virtual void	LoadDataToGPU();
		virtual void	SetCamera(GLCamera *newCamera);

		unsigned int	GetNumElementsSelected();

		void	SetTerrainLayer(TerrainLayer* newLayer);
		void	UseTool(int toolID);

		void	MouseClick(int x, int y);
		void	MouseMove(int x, int y);
		void	MouseRelease(int x, int y);

		void	Undo();
		void	Redo();

	protected:

		/* Layers */
		TerrainLayer*	terrainLayer;	/**< The Terrain Layer that we'll get the subdomain from */

		/* Selection Tools */
		int		activeTool;	/**< Integer used to determine which tool mouse actions are sent to */
		CircleTool*	circleTool;	/**< Tool for selecting elements inside of a circle */

		/* Selected Elements */
		std::vector<Element*>*		selectedElements;	/**< The list of currently selected Elements */

		/* Undo and Redo Stacks */
		std::stack<std::vector<Element*>*, std::vector<std::vector<Element*>*> >	undoStack;
		std::stack<std::vector<Element*>*, std::vector<std::vector<Element*>*> >	redoStack;

		/* OpenGL Variables */
		bool		glLoaded;	/**< Flag that shows if the VAO/VBO/IBO have been created */
		GLCamera*	camera;		/**< The camera used to draw selections */
		GLuint		VAOId;		/**< The vertex array object ID in the OpenGL context */
		GLuint		VBOId;		/**< The vertex buffer object ID in the OpenGL context */
		GLuint		IBOId;		/**< The index buffer object ID in the OpenGL context */
		SolidShader*	outlineShader;	/**< The shader used to draw Element outlines */
		SolidShader*	fillShader;	/**< The shader used to draw Element fill */
		SolidShader*	boundaryShader;	/**< The shader used to draw the Subdomain boundary */

		/* Flags */
		bool	mousePressed;

		/* OpenGL Functions */
		void	InitializeGL();

		/* Tool Initialization Functions */
		void	CreateCircleTool();

	signals:

		void	UndoAvailable(bool);		/**< Triggered when an undo action becomes available/unavailable */
		void	RedoAvailable(bool);		/**< Triggered when a redo action becomes available/unavailable */
		void	Refreshed();			/**< Triggered when new data has been loaded to the OpenGL context */
		void	NumElementsSelected(int);	/**< Triggered when the number of selected elements changes */

	protected slots:

		void	CircleToolFinishedSearching();
};

#endif // CREATIONSELECTIONLAYER_H
