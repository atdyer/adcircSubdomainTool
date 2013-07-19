#ifndef DOMAIN_H
#define DOMAIN_H

#include <QObject>
#include <QThread>
#include <QProgressBar>

#include <string>
#include <vector>

#include "Layers/Layer.h"
#include "Layers/TerrainLayer.h"
#include "Layers/SelectionLayer.h"

#include "SubdomainTools/CircleTool.h"

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/GradientShader.h"



/**
 * @brief This class is used to represent an ADCIRC domain (either a full or subdomain).
 *
 * This class is used to represent an ADCIRC domain (either a full or subdomain). An instance
 * of this class contains everything needed to hold all of the data associated with a single
 * ADCIRC run.
 *
 * It is a very high level class, acting as a sort of umbrella used to oversee all functionality
 * and organization of the large amount of data it contains. As such, it provides an excellent
 * bridge between the GUI and the underlying data.
 *
 * <H2>Domain Independence</H2>
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
 */
class Domain : public QObject
{
		Q_OBJECT
	public:
		Domain();
		~Domain();

		// OpenGL Panel Drawing and Selection Interaction Functions
		void	Draw();
		void	Zoom(float zoomAmount);
		void	Pan(float dx, float dy);
		void	SetWindowSize(float w, float h);
		void	SetCircleToolCenter(int x, int y);
		void	SetCircleToolRadius(int x, int y);
		void	SetCircleToolFinished();
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


	protected:

		// Camera
		GLCamera*	camera;		/**< The camera used for all drawing operations (except the selection layer)*/

		// Layers
		SelectionLayer*	selectionLayer;	/**< The selection layer used for drawing selected nodes/elements */
		TerrainLayer*	terrainLayer;	/**< The terrain layer */

		// Tools
		CircleTool*	circleTool;	/**< The circle tool used to select nodes/elements inside of a user-drawn circle */

		// Loading Operations
		QThread*	layerThread;	/**< The thread on which file reading operations will execute */
		QProgressBar*	progressBar;	/**< The progress bar that will show file reading progress */
		Layer*		loadingLayer;	/**< Sort of a queue for the next layer that will send data to the GPU */


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

	public slots:

		void	setMouseCoordinates(float x, float y);

	protected slots:

		void	LoadLayerToGPU();

};

#endif // DOMAIN_H
