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
		GLCamera*	GetCurrentCamera();
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
		void	UseSolidOutline(unsigned int layerID);
		void	UseSolidFill(unsigned int layerID);


	protected:

		QThread*		layerThread;	/**< The thread that all Layer slots will operate on */
		GLCamera*		currentCam;	/**< The camera to be used by all shaders */


		std::vector<GLCamera*>		cameras;	/**< MEMORY MANAGEMENT - List of all GLCamera objects */
		std::vector<TerrainLayer*>	terrainLayers;	/**< MEMORY MANAGEMENT - List of all TerrainLayer objects */
		std::vector<SolidShader*>	solidShaders;	/**< MEMORY MANAGEMENT - List of all SolidShader objects*/

		std::vector<Layer*>		allLayers;	/**< MASS ACCESS - List of all Layer objects */
		std::vector<GLShader*>		allShaders;	/**< MASS ACCESS - List of all GLShader objects*/

		std::vector<Layer*>		visibleLayers;		/**< REFERENCE TABLE - List of visible Layers */
		std::vector<Layer*>		hiddenLayers;		/**< REFERENCE TABLE - List of hidden Layers */
		std::vector<SolidShader*>	solidOutlineShaders;	/**< REFERENCE TABLE - List of SolidShader objects used as outlines for each Layer */
		std::vector<SolidShader*>	solidFillShaders;	/**< REFERENCE TABLE - List of SolidShdaer objects used as fill for each Layer */

		unsigned int	AddReferenceTableSlot();
		int		GetReferenceTableColumn(unsigned int layerID);

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
