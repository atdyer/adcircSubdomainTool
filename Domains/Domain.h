#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QThread>
#include <QProgressBar>
#include <QMouseEvent>
#include <QWheelEvent>

#include <string>
#include <vector>

#include "Layers/Layer.h"
#include "Layers/TerrainLayer.h"
#include "Layers/SelectionLayers/CreationSelectionLayer.h"

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/GradientShader.h"


/**
 * @brief This class is used to represent an ADCIRC domain (either a full or subdomain).
 *
 * This class is used to represent an ADCIRC domain (either a full or subdomain). An instance
 * of this class contains everything needed to load and interact with all of the data associated
 * with a single ADCIRC run.
 *
 * It is a very high level class, acting as a sort of umbrella used to oversee all functionality
 * and organization of the large amount of data it contains. As such, it provides an excellent
 * bridge between the GUI and the underlying data.
 *
 * <H2>Independence</H2>
 *
 * One of the goals of the Subdomain Modeling Tool is to give the user the ability to easily
 * manipulate many subdomains independently of one another in a single GUI. Therefore,
 * the following design decisions were made:
 * - Each Domain has its own GLCamera object, which allows the user to toggle between domains
 *   without changing the view of the individual domains
 * - Each Domain has its own SelectionLayer, which not only allows the user to maintain selections
 *   independently, but also provides an Undo/Redo stack for each Domain. This means that a user
 *   can make changes in one domain and switch to a different domain and undo changes that had
 *   previously occurred in that domain
 *
 * <H2>Qt Signal/Slot Functionality</H2>
 *
 * This class has been developed with the idea that it be used in the context of a Qt signal/slot
 * mechanism. However, all basic functionality has been written using basic functions so that
 * upon removal from the Qt environment, the class will still perform as expected.
 *
 * All of the signals and slots used by this class are only used to communicate state to GUI. Any
 * function that modifies the state of the class is implemented as a normal function.
 *
 */
class Domain : public QObject
{
		Q_OBJECT
	public:
		Domain();
		~Domain();

		// Drawing and Selection Interaction Functions
		void	Draw();
		void	MouseClick(QMouseEvent *event);
		void	MouseMove(QMouseEvent *event);
		void	MouseRelease(QMouseEvent *event);
		void	MouseWheel(QWheelEvent *event);
		void	SetWindowSize(float w, float h);
		void	UseTool(ToolType tool, SelectionType selection);
		void	Undo();
		void	Redo();

		// Modification functions used to set the state of the Domain based on GUI interaction
		void	SetProgressBar(QProgressBar* newBar);
		void	SetFort14Location(std::string newLoc);
		void	SetFort15Location(std::string newLoc);
		void	SetFort63Location(std::string newLoc);
		void	SetFort64Location(std::string newLoc);
		void	SetTerrainSolidOutline(SolidShaderProperties newProperties);
		void	SetTerrainSolidFill(SolidShaderProperties newProperties);
		void	SetTerrainGradientOutline(GradientShaderProperties newProperties);
		void	SetTerrainGradientFill(GradientShaderProperties newProperties);

		// Query functions used to access data used to populate the GUI
		std::string	GetFort14Location();
		std::string	GetFort15Location();
		std::string	GetFort63Location();
		std::string	GetFort64Location();
		SolidShaderProperties		GetTerrainSolidOutline();
		SolidShaderProperties		GetTerrainSolidFill();
		GradientShaderProperties	GetTerrainGradientOutline();
		GradientShaderProperties	GetTerrainGradientFill();
		unsigned int	GetNumNodesDomain();
		unsigned int	GetNumElementsDomain();
		unsigned int	GetNumNodesSelected();
		unsigned int	GetNumElementsSelected();


	private:

		// Camera
		GLCamera*	camera;		/**< The camera used for all drawing operations (except the selection layer)*/

		// Layers
		TerrainLayer*		terrainLayer;	/**< The terrain layer */
		CreationSelectionLayer*	selectionLayer;	/**< The selection layer */

		// Loading Operations
		QThread*	layerThread;	/**< The thread on which file reading operations will execute */
		QProgressBar*	progressBar;	/**< The progress bar that will show file reading progress */
		Layer*		loadingLayer;	/**< Sort of a queue for the next layer that will send data to the GPU */

		/* Mouse Clicking and Moving Stuff */
		ActionType	currentMode;	/**< The current mode used to determine where actions are sent */
		int		oldx;	/**< The old x-coordinate of the mouse */
		int		oldy;	/**< The old y-coordinate of the mouse */
		int		newx;	/**< The new x-coordinate of the mouse */
		int		newy;	/**< The new y-coordinate of the mouse */
		int		dx;	/**< The change in x-coordinates between mouse events */
		int		dy;	/**< The change in y-coordinates between mouse events */
		Qt::MouseButton	pushedButton;	/**< The mouse button being pushed */
		bool		clicking;	/**< Flag that shows if a mouse button is being held down */
		bool		mouseMoved;	/**< Flag that shows if the mouse has moved since the last event */
		void		Zoom(float zoomAmount);
		void		Pan(float dx, float dy);
		void		CalculateMouseCoordinates();


	signals:

		void	mouseX(float);		/**< Emits the mouse x-coordinate in domain space as the mouse is moved */
		void	mouseY(float);		/**< Emits the mouse y-coordinate in domain space as the mosue is moved */
		void	undoAvailable(bool);	/**< Emitted when an undo action becomes available or unavailable */
		void	redoAvailable(bool);	/**< Emitted when a redo action becomes available or unavailable */

		void	numNodesDomain(int);		/**< Emitted when the number of nodes in the domain changes */
		void	numElementsDomain(int);		/**< Emitted when the number of elements in the domain changes */
		void	numNodesSelected(int);		/**< Emitted when the number of currently selected nodes changes */
		void	numElementsSelected(int);	/**< Emitted when the number of currently selected elements changes */

		/* Selection Tool Pass-through Signals */
		void	circleToolStatsSet(float, float, float);	/**< Emitted when the circle tool circle changes size */
		void	circleToolStatsFinished();			/**< Emitted when the circle tool has stopped drawing the circle */

		void	beingDestroyed();	/**< Emitted when the destructor is first called */
		void	emitMessage(QString);	/**< Emitted any time a text message needs to be passed to the GUI */
		void	updateGL();		/**< Emitted any time the OpenGL context needs to be redrawn */

	protected slots:

		void	LoadLayerToGPU();
		void	ToolFinishedDrawing();

};

#endif // DOMAIN_H
