#ifndef TERRAINLAYER_H
#define TERRAINLAYER_H

#include "adcData.h"
#include "Quadtree.h"
#include "Layer.h"
#include "OpenGL/Shaders/GLShader.h"

#include <string>
#include <vector>
#include <QThread>


/**
 * @brief This class is used to read a display terrain data from a fort.14 file
 *
 * This class is responsible for all data associated with an ADCIRC fort.14 file. It is
 * capable of reading the file, storing the data, and quickly accessing and drawing the
 * data as needed.
 *
 */
class TerrainLayer : public Layer
{
		Q_OBJECT
	public:

		// Constructor/Destructor
		TerrainLayer();
		~TerrainLayer();

		// Virtual methods to override
		virtual void	Draw();

		// Getter Methods
		std::string	GetFort14Location();
		Node*		GetNode(unsigned int nodeNumber);
		Node*		GetNode(float x, float y);
		Element*	GetElement(unsigned int elementNumber);
		Element*	GetElement(float x, float y);
		unsigned int	GetNumNodes();
		unsigned int	GetNumElements();
		float		GetMinX();
		float		GetMaxX();
		float		GetMinY();
		float		GetMaxY();
		float		GetMinZ();
		float		GetMaxZ();

		// Setter Methods
		void	SetFort14Location(std::string newLocation);
		void	SetOutlineShader(GLShader *newShader);
		void	SetFillShader(GLShader *newShader);

	protected:

		// Terrain Specific Variables
		std::string		fort14Location; /**< The absolute path of the fort.14 file */
		std::vector<Node>	nodes;		/**< List of all Nodes in the Layer */
		std::vector<Element>	elements;	/**< List of all Elements in the Layer */
		unsigned int		numNodes;	/**< The number of Nodes in the Layer as specified in fort.14 */
		unsigned int		numElements;	/**< The number of Elements in the Layer as specified in fort.14 */
		float			minX;		/**< The minimum x-value */
		float			maxX;		/**< The maximum x-value */
		float			minY;		/**< The minimum y-value */
		float			maxY;		/**< The maximum y-value */
		float			minZ;		/**< The minimum z-value */
		float			maxZ;		/**< The maximum z-value */

		// Terrain Specific OpenGL Variables
		GLuint		VAOId;			/**< The vertex array object ID in the OpenGL context */
		GLuint		VBOId;			/**< The vertex buffer object ID in the OpenGL context */
		GLuint		IBOId;			/**< The index buffer object ID in the OpenGL context */
		GLShader*	outlineShader;		/**< The shader used to draw outlines */
		GLShader*	fillShader;		/**< The shader used to draw fill */

		// Flags
		bool	flipZValue;		/**< Flag that determines if the z-value is multiplied by -1 before being loaded to the GPU */
		bool	normalizeCoords;	/**< Flag that determines if the xy-coords will be normalized before begin loaded to the GPU */
		bool	fileLoaded;		/**< Flag that shows if data has been successfully read from the fort.14 file */

		// Picking Variables
		Quadtree*	quadtree;	/**< The quadtree used for Node picking */

	public slots:

		// Slots used for work that needs to be performed on a worker thread
		void	readFort14();

	signals:

		// Signals used during threaded reading of fort.14
		void	fort14Valid();
		void	startedReadingFort14();
		void	progress(int percentage);
		void	finishedReadingFort14();
		void	error(QString err);
};

#endif // TERRAINLAYER_H
