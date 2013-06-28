#include "TerrainLayer.h"

TerrainLayer::TerrainLayer()
{
	fort14Location = "";
	numNodes = 0;
	numElements = 0;
	minX = 0.0;
	maxX = 0.0;
	minY = 0.0;
	maxY = 0.0;
	minZ = 0.0;
	maxZ = 0.0;

	flipZValue = true;
	normalizeCoords = true;
	fileLoaded = false;

	outlineShader = 0;
	fillShader = 0;
}


TerrainLayer::~TerrainLayer()
{
	if (fileLoaded)
	{
		glBindVertexArray(VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glPolygonOffset(6, 6);
			fillShader->Use();
			glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(4, 4);
			outlineShader->Use();
			glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


/**
 * @brief Attempts to draw as much of the layer's data as possible
 *
 * This function attempts to draw the layer's data using each of the defined shaders
 *
 */
void TerrainLayer::Draw()
{
	std::cout << "Drawing" << std::endl;
}


/**
 * @brief Getter method that returns the fort.14 location
 * @return The fort.14 absolute path
 */
std::string TerrainLayer::GetFort14Location()
{
	return fort14Location;
}


/**
 * @brief Returns a pointer to the Node with the corresponding node number
 *
 * This functions provides access to Nodes in the node list through node number.
 * The Node is first looked up on the assumption that the Node list is ordered,
 * but if the list is not ordered, a linear lookup on the Node list is performed
 * until the correct Node is found.
 *
 * @param nodeNumber The node number as defined in the fort.14 file
 * @return A pointer to the Node with the corresponding node number
 * @return 0 if the Node is not in the node list
 */
Node* TerrainLayer::GetNode(unsigned int nodeNumber)
{
	if (nodes.size() > 0 && nodeNumber <= nodes.size() && nodeNumber == nodes[nodeNumber-1].nodeNumber)
		return &nodes[nodeNumber-1];
	else
		for (unsigned int i=0; i<nodes.size(); i++)
			if (nodes[i].nodeNumber == nodeNumber)
				return &nodes[i];
	return 0;
}


/**
 * @brief Returns a pointer to the Node closest to the provided x-y coordinates
 *
 * This function provides access to Nodes in the node list by finding the Node
 * with the x-y coordinates closest to the provided x-y coordinates. A quick
 * lookup is possible by parsing the Quadtree used to map the Node data.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Node closest to the provided x-y coordinates
 * @return 0 if the click was outside the bounds of the quadtree or if there is no data loaded
 */
Node* TerrainLayer::GetNode(float x, float y)
{
	if (quadtree)
		return quadtree->FindNode(x, y);
	else
		return 0;
}


/**
 * @brief Returns a pointer to the Element with the corresponding element number
 *
 * This function provides access to Elements in the element list through element
 * number. The element is first looked up on the assumption that the Element
 * list is ordered, but if the list is not ordered, a linear lookup on the
 * Element list is performed until the correct Element is found.
 *
 * @param elementNumber The element number as defined in the fort.14 file
 * @return A pointer to the Element with the corresponding element number
 * @return 0 if the Element is not in the element list
 */
Element* TerrainLayer::GetElement(unsigned int elementNumber)
{
	if (elements.size() > 0 && elementNumber <= elements.size() && elementNumber == elements[elementNumber-1].elementNumber)
		return &elements[elementNumber-1];
	else
		for (unsigned int i=0; i<elements.size(); i++)
			if (elements[i].elementNumber == elementNumber)
				return &elements[i];
	return 0;
}


/**
 * @brief Returns a pointer to the Element closest to the provided x-y coordinates
 *
 * This function provides access to Elements in the element list by finding the Element
 * closest to the provided x-y coordinates. If the x-y coordinates are inside of an
 * Element, that Element will be returned. If the coordinates do not fall inside of
 * any Element in the list, 0 is returned.
 *
 * @param x The x-coordinate
 * @param y The y-coordinate
 * @return A pointer to the Element that contains the provided x-y coordinates
 * @return 0 if the element list is empty or if the point is not inside of any Element
 */
Element* TerrainLayer::GetElement(float x, float y)
{
	if (x || y)
		return 0;
	return 0;
}


/**
 * @brief Get the number of nodes
 * @return The number of nodes
 */
unsigned int TerrainLayer::GetNumNodes()
{
	return numNodes;
}


/**
 * @brief Get the number of elements
 * @return The number of elements
 */
unsigned int TerrainLayer::GetNumElements()
{
	return numElements;
}


/**
 * @brief Get the minimum x-coordinate
 * @return The minimum x-coordinate
 */
float TerrainLayer::GetMinX()
{
	return minX;
}


/**
 * @brief Get the maximum x-coordinate
 * @return The maximum x-coordinate
 */
float TerrainLayer::GetMaxX()
{
	return maxX;
}


/**
 * @brief Get the minimum y-coordinate
 * @return The minimum y-coordinate
 */
float TerrainLayer::GetMinY()
{
	return minY;
}


/**
 * @brief Get the maximum y-coordinate
 * @return The maximum y-coordinate
 */
float TerrainLayer::GetMaxY()
{
	return maxY;
}


/**
 * @brief Get the minimum z-coordinate
 * @return The minimum z-coordinate
 */
float TerrainLayer::GetMinZ()
{
	return minZ;
}


/**
 * @brief Get the maximum z-coordinate
 * @return The maximum z-coordinate
 */
float TerrainLayer::GetMaxZ()
{
	return maxZ;
}


/**
 * @brief Sets the location of the fort.14 file for this layer and checks its validity
 *
 * This function will store the given location of the fort.14 file and then attempt to
 * check the validity of the path given. Once it has verified that the file is valid,
 * it emits the fort14Valid() signal.
 *
 * @param newLocation The absolute path of the fort.14 file
 */
void TerrainLayer::SetFort14Location(std::string newLocation)
{
	fort14Location = newLocation;

	// Check fort.14 here


	emit fort14Valid();
}


/**
 * @brief Sets the GLShader that will be used when drawing the outlines of this layer
 *
 * Set the GLShader to be used when drawing the outlines of this layer. Note that the
 * layer does not take ownership of the shader.
 *
 * @param newShader The GLShader to be used for drawing outlines
 */
void TerrainLayer::SetOutlineShader(GLShader *newShader)
{
	outlineShader = newShader;
}


/**
 * @brief Sets the GLShader that will be used when drawing the fill for this layer
 *
 * Set the GLShader to be used when drawing the fill for this layer. Note that the
 * layer does not take ownership of the shader.
 *
 * @param newShader The GLShader to be used for drawing outlines
 */
void TerrainLayer::SetFillShader(GLShader *newShader)
{
	outlineShader = newShader;
}


/**
 * @brief Reads the fort.14 file data
 *
 * This function is used to read data from the fort.14 file. It is implemented as
 * a slot so that it can be part of the signal/slot mechanism used for threading.
 * Typical threaded usage of this function:
 * - Create TerrainLayer object on heap (using new)
 * - Create new QThread
 * - Move the TerrainLayer object to the
 *
 */
void TerrainLayer::readFort14()
{
	std::ifstream fort14 (fort14Location.data());

	if (fort14.is_open())
	{
		emit startedReadingFort14();

		std::string line;
		std::getline(fort14, infoLine);
		std::getline(fort14, line);
		std::stringstream(line) >> numElements >> numNodes;

		// Progress bar stuff
		int linesRead = 0;
		int linesToRead = numElements+numNodes;

		if (numNodes > 0 && numElements > 0)
		{
			nodes.reserve(numNodes);
			elements.reserve(numElements);

			Node currNode;
			for (unsigned int i=0; i<numNodes; i++)
			{
				fort14 >> currNode.nodeNumber;
				fort14 >> currNode.x;
				fort14 >> currNode.y;
				fort14 >> currNode.z;
				if (flipZValue)
					currNode.z *= -1.0;
				nodes.push_back(currNode);
				emit progress((++linesRead)/linesToRead);
			}

			Element currElement;
			int trash;
			for (unsigned int i=0; i<numElements; i++)
			{
				fort14 >> currElement.elementNumber;
				fort14 >> trash;
				fort14 >> currElement.n1;
				fort14 >> currElement.n2;
				fort14 >> currElement.n3;
				elements.push_back(currElement);
				emit progress((++linesRead)/linesToRead);
			}

			fort14.close();

			fileLoaded = true;

			emit finishedReadingFort14();
			emit emitMessage(QString("Terrain layer created: <strong>").append(infoLine.data()).append("</strong>"));
		}
	} else {
		fileLoaded = false;
		emit emitMessage("Error opening fort.14 file");
	}

//	// Simulate a file being read
//	emit startedReadingFort14();
//	for (unsigned int i=0; i<100; i++)
//	{
//		usleep(10000);
//		emit progress(i+1);
//	}
//	emit finishedReadingFort14();
//	emit emitMessage(QString("Terrain layer created"));
}
