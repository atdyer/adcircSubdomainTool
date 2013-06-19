#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QObject>
#include <QThread>
#include <QProgressBar>

#include <vector>

#include "Layer.h"
#include "TerrainLayer.h"

#include <OpenGL/GLCamera.h>
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

		// Camera functions
		GLCamera	*GetCurrentCamera();
		void		SwitchToCamera(unsigned int camID);

		// Drawing Functions
		void	DrawVisibleLayers();

		// Layer Displaying Functions
		void	ShowLayer(unsigned int layerID);
		void	ShowOnlyLayer(unsigned int layerID);
		void	HideLayer(unsigned int layerID);

		// Layer Creation Functions
		unsigned int	CreateNewTerrainLayer(std::string fort14Location, QProgressBar* progressBar = 0);

		// Shader Functions
		void	PairOutlineShader(unsigned int layerID, unsigned int shaderID);
		void	PairFillShader(unsigned int layerID, unsigned int shaderID);


	protected:

		QThread*		LayerThread;	/**< The thread that all Layer slots will operate on */

		GLCamera*		currentCam;	/**< The camera to be used by all shaders */
		std::vector<GLCamera*>	cameras;	/**< List of all Camera objects */

		std::vector<Layer*>		visibleLayers;	/**< Ordered list of visible Layers */
		std::vector<Layer*>		hiddenLayers;	/**< Ordered list of hidden Layers */
		std::vector<TerrainLayer*>	terrainLayers;	/**< List of all TerrainLayer objects */

		std::vector<GLShader*>		allShaders;	/**< List of all GLShader objects */
		std::vector<SolidShader*>	solidShaders;	/**< List of all SolidShader objects */

		Layer*		GetLayerByID(unsigned int layerID);
		GLShader*	GetShaderByID(unsigned int shaderID);

		void	UpdateShaderCameras();

		SolidShader*	NewSolidShader(float r, float g, float b, float a);

	signals:

		void	emitMessage(QString);
		void	cameraChanged();
		void	beingDestroyed();
};

#endif // LAYERMANAGER_H
