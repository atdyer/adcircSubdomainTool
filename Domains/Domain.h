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

		// OpenGL Panel Drawing and Interaction Functions
		void	Draw();
		void	Zoom(float zoomAmount);
		void	Pan(float dx, float dy);
		void	SetWindowSize(float w, float h);
		void	SetCircleToolCenter(int x, int y);
		void	SetCircleToolRadius(int x, int y);
		void	SetCircleToolFinished();

		// Modification functions used to set the state of the Domain based on GUI interaction
		void	SetFort14Location(std::string newLoc);
		void	SetFort15Location(std::string newLoc);
		void	SetFort63Location(std::string newLoc);
		void	SetFort64Location(std::string newLoc);
		void	SetProgressBar(QProgressBar* newBar);


		// Query functions used to access data used to populate the GUI
		GLCamera*	GetCamera();
		std::string	GetFort14Location();
		std::string	GetFort15Location();
		std::string	GetFort63Location();
		std::string	GetFort64Location();
		SolidShaderProperties		GetTerrainSolidOutline();
		SolidShaderProperties		GetTerrainSolidFill();
		GradientShaderProperties	GetTerrainGradientOutline();
		GradientShaderProperties	GetTerrainGradientFill();


	protected:

		// Camera
		GLCamera*	camera;

		// Layers
		SelectionLayer*	selectionLayer;
		TerrainLayer*	terrainLayer;

		// Tools
		CircleTool*	circleTool;

		// Loading Operations
		QThread*	layerThread;
		QProgressBar*	progressBar;
		Layer*		loadingLayer;


	signals:

		void	beingDestroyed();
		void	emitMessage(QString);
		void	updateGL();
		void	domainRefreshed();

	protected slots:

		void	LoadLayerToGPU();

};

#endif // DOMAIN_H
