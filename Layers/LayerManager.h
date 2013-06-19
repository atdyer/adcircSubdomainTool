#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QThread>
#include <QProgressBar>

#include <vector>

#include "Layer.h"
#include "TerrainLayer.h"

#include <OpenGL/OpenGLPanel.h>
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"

/**
 * @brief This class is used to keep track of all Layers and Shaders and to manage
 * all threading and allocated memory that the Layers and Shaders use.
 */
class LayerManager : public QObject
{
		Q_OBJECT
	public:

		LayerManager(QObject* parent=0);
		~LayerManager();

		// Required initialization function
		void	Initialize(OpenGLPanel* panel);

		// Camera functions
		void	SwitchToCamera(unsigned int cam);

		// Drawing Functions
		void	DrawVisibleLayers();

		// Layer Displaying Functions
		void	DisplayLayer(unsigned int layerID);
		void	DisplayLayerUnique(unsigned int layerID);
		void	ShowLayer(unsigned int layerID);
		void	HideLayer(unsigned int layerID);

		// Layer Creation Functions
		unsigned int	CreateNewTerrainLayer(std::string fort14Location, QProgressBar* progressBar = 0);

		// Shader Functions
		void	PairOutlineShader(unsigned int layerID, unsigned int shaderID);
		void	PairFillShader(unsigned int layerID, unsigned int shaderID);


	protected:

		Layer*		GetLayerByID(unsigned int layerID);
		GLShader*	GetShaderByID(unsigned int shaderID);

		QThread*		LayerThread;	/**< The thread that all Layer slots will operate on */

		OpenGLPanel*		GLPanel;	/**< The OpenGLPanel on the UI that displays everything */
		GLCamera*		currentCam;	/**< The camera to be used by all shaders */
		std::vector<GLCamera*>	cameras;	/**< List of all Camera objects */

		std::vector<Layer*>		visibleLayers;	/**< Ordered list of visible Layers */
		std::vector<Layer*>		hiddenLayers;	/**< Ordered list of hidden Layers */
		std::vector<TerrainLayer*>	terrainLayers;	/**< List of all TerrainLayer objects */

		std::vector<SolidShader*>	solidShaders;	/**< List of all SolidShader objects */

	signals:

		void	emitMessage(QString);
		void	beingDestroyed();
};

#endif // LAYERMANAGER_H
