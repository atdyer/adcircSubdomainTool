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
 * @brief This class is meant to provide a layer of abstraction between the UI and the numerous
 * objects that need to be created and organized in order to properly draw ADCIRC data in an
 * OpenGL context.
 *
 * The LayerManager class is meant to provide a layer of abstraction between the UI and the numerous
 * objects that need to be created and organized in order to properly draw ADCIRC data in an
 * OpenGL context.
 *
 * <H2>Layer/Shader Organization</H2>
 *
 * It provides a number of abstractions, the most important being the REFERENCE TABLE, which can be though of
 * as a simple table where the columns represent the different ADCIRC layers that need to be drawn and the rows
 * represent all of the different types ofshading programs that are used to draw the data. Here is and example that
 * shows the basic structure of the table:
 *
 * <TABLE>
 * <TR style="background-color: #B8B8B8">
 * <TD><STRONG>Visible Layers</STRONG></TD>		<TD>*Layer1</TD>	<TD></TD>	<TD></TD>		<TD>*Layer4</TD>
 * </TR>
 * <TR style="background-color: #B8B8B8">
 * <TD><STRONG>Hidden Layers</STRONG></TD>		<TD></TD>		<TD>*Layer2</TD><TD>*Layer3</TD>	<TD> </TD>
 * </TR>
 * <TR>
 * <TD><STRONG>Solid Outline Shader</STRONG></TD>	<TD>*shdr1</TD>		<TD>*shdr2</TD>	<TD>*shdr3</TD>		<TD>*shdr4</TD>
 * </TR>
 * <TR>
 * <TD><STRONG>Solid Fill Shader</STRONG></TD>		<TD>*shdr5</TD>		<TD>*shdr6</TD>	<TD>*shdr7</TD>		<TD>*shdr8</TD>
 * </TR>
 * <TR>
 * <TD><STRONG>Gradient Fill Shader</STRONG></TD>	<TD></TD>		<TD></TD>	<TD>*shdr9</TD>		<TD></TD>
 * </TR>
 * </TABLE>
 *
 * When a new Layer is created, a new column is added to the table (by calling LayerManager::AddReferenceTableSlot()) and
 * the pointer to the new Layer is either put in the Visible Layers or Hidden Layers row.
 *
 * Next, any shaders that are needed to initially display the layer are created and their pointers are put into the
 * appropriate rows. In this example, each of the four layers has a solid fill and a solid outline shader, but only
 * Layer3 has a gradient shader.
 *
 * The reasoning behind this structure is that it gives us an easy way to update the user interface with appropriate data
 * based on the options that the user selects by providing quick, organized access to highly specific classes. It also
 * allows us to save the state of data that is not currently being displayed to the user interface without disrupting
 * the state of the data.
 *
 * <H2>Memory Management</H2>
 *
 */
class LayerManager : public QObject
{
		Q_OBJECT
	public:

		LayerManager(QObject* parent=0);
		~LayerManager();

		// Basic Getter Functions
		float		GetMouseX(float x);
		float		GetMouseY(float y);

		// Camera Functions
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
		void	UseSolidOutlineShader(unsigned int layerID);
		void	UseSolidFillShader(unsigned int layerID);
		SolidShader*	GetSolidOutlineShader(unsigned int layerID);
		SolidShader*	GetSolidFillShader(unsigned int layerID);


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

	private:

		Layer*	loadingLayer;

	signals:

		void	emitMessage(QString);
		void	numNodesChanged(int);
		void	numElementsChanged(int);
		void	numTSChanged(int);
		void	cameraChanged();
		void	updateGL();
		void	beingDestroyed();

		void	activeTerrainLayer(TerrainLayer*);

	public slots:

		void	LoadToGPU();
};

#endif // LAYERMANAGER_H
