#include "Fort14Reader.h"

Fort14Reader::Fort14Reader(QString fileLoc, std::vector<Node> *nodeList, std::vector<Element> *elementList, std::vector<unsigned int> *boundaryList, Quadtree *quadtree, bool normalize, QObject *parent) :
	QObject(parent),
	boundaryNodes(boundaryList),
	currProgress(0),
	elements(elementList),
	fullProgress(0),
	nodes(nodeList),
	normalizeCoordinates(normalize),
	quadtree(quadtree),
	targetFile(fileLoc)
{

}


Fort14Reader::~Fort14Reader()
{
	std::cout << "Deleting fort.14 reader" << std::endl;
}


void Fort14Reader::ReadFile()
{
	emit StartedReading();

	std::ifstream fort14 (targetFile.toStdString().data());

	if (fort14.is_open() && nodes && elements)
	{
		nodes->clear();
		elements->clear();

		std::string infoLine, line;
		int numElements, numNodes;

		std::getline(fort14, infoLine);
		std::getline(fort14, line);
		std::stringstream(line) >> numElements >> numNodes;

		nodes->reserve(numNodes);
		elements->reserve(numElements);

		currProgress = 0;
		fullProgress = 3*numNodes + 2*numElements;

		if (ReadNodalData(numNodes, &fort14))
		{
			if (ReadElementalData(numElements, &fort14))
			{
				ReadBoundaries(&fort14);
				if (normalizeCoordinates)
				{
					NormalizeCoordinates();
				}
				PopulateQuadtree();
			}
		}
	}

	emit FinishedReading();
}


Node* Fort14Reader::GetNode(unsigned int nodeNumber)
{
	if (nodes.size() > 0 && nodeNumber <= nodes.size() && nodeNumber == nodes[nodeNumber-1].nodeNumber)
		return &nodes[nodeNumber-1];
	else
		for (unsigned int i=0; i<nodes.size(); i++)
			if (nodes[i].nodeNumber == nodeNumber)
				return &nodes[i];
	return 0;
}


void Fort14Reader::NormalizeCoordinates()
{

}


void Fort14Reader::PopulateQuadtree()
{

}


void Fort14Reader::ReadBoundaries(std::ifstream *fileStream)
{
	if (boundaryNodes)
	{
		std::string line;
		int numSegments, numBoundaryNodes;
		std::getline(*fileStream, line);
	}
}


bool Fort14Reader::ReadElementalData(int numElements, std::ifstream *fileStream)
{
	Element currElement;
	unsigned int trash, currNodeNumber;
	for (int i=0; i<numElements; ++i)
	{
		*fileStream >> currElement.elementNumber;
		*fileStream >> trash;
		*fileStream >> currNodeNumber;
		currElement.n1 = GetNode(currNodeNumber);
		*fileStream >> currNodeNumber;
		currElement.n2 = GetNode(currNodeNumber);
		*fileStream >> currNodeNumber;
		currElement.n3 = GetNode(currNodeNumber);
		elements->push_back(currElement);
		emit Progress(100*(++currProgress)/fullProgress);
	}
}


bool Fort14Reader::ReadNodalData(int numNodes, std::ifstream *fileStream)
{
	float minX, minY, minZ, maxX, maxY, maxZ;
	minX = minY = minZ = 99999.0;
	maxX = maxY = maxZ = -99999.0;

	Node currNode;
	for (int i=0; i<numNodes; ++i)
	{
		*fileStream >> currNode.nodeNumber;
		*fileStream >> currNode.xDat;
		*fileStream >> currNode.yDat;
		*fileStream >> currNode.zDat;
		currNode.x = atof(currNode.xDat.data());
		currNode.y = atof(currNode.yDat.data());
		currNode.z = -1.0*atof(currNode.zDat.data());

		if (currNode.x < minX)
			minX = currNode.x;
		else if (currNode.x > maxX)
			maxX = currNode.x;
		if (currNode.y < minY)
			minY = currNode.y;
		else if (currNode.y > maxY)
			maxY = currNode.y;
		if (currNode.z < minZ)
			minZ = currNode.z;
		else if (currNode.z > maxZ)
			maxZ = currNode.z;

		nodes->push_back(currNode);
		emit Progress(100*(++currProgress)/fullProgress);
	}
}



