#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QThread>
#include <QProgressBar>
#include <vector>

#include "Layer.h"
#include "TerrainLayer.h"

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

		QThread*		LayerThread;	/**< The thread that all Layer slots will operate on */

		std::vector<Layer*>		visibleLayers;	/**< Ordered list of visible Layers */
		std::vector<Layer*>		hiddenLayers;	/**< Ordered list of hidden Layers */
		std::vector<TerrainLayer*>	terrainLayers;	/**< List of all TerrainLayer objects */

	signals:

		void	emitMessage(const char*);
		void	beingDestroyed();
};

#endif // LAYERMANAGER_H
