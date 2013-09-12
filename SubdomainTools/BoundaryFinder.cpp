#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder()
{

}


BoundaryFinder::~BoundaryFinder()
{

}


std::vector<unsigned int> BoundaryFinder::FindBoundaries(ElementState *elementSelection)
{
	edgesMap.clear();
	nodeAdjacency.clear();
	edgesList.clear();
	FindEdges(elementSelection->GetState());
	CreateEdgesList();
	return edgesList;
}


std::vector<unsigned int> BoundaryFinder::FindInnerBoundaries(ElementState *elementSelection)
{
	std::vector<unsigned int> innerBoundaryNodes;
	if (elementSelection)
	{
		std::vector<Element*> *selectedElements = elementSelection->GetState();
		std::map<Edge, int> edgeInElementCount;
		std::map<unsigned int, std::vector<Element*> > elementsThatContainNode;

		/* Make a map of the number of times every edge appears in the domain */
		/* Make a map of the list of Elements that every node is a part of */
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements->begin(); it != selectedElements->end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				Edge edge1 (currElement->n1->nodeNumber, currElement->n2->nodeNumber);
				Edge edge2 (currElement->n1->nodeNumber, currElement->n3->nodeNumber);
				Edge edge3 (currElement->n2->nodeNumber, currElement->n3->nodeNumber);
				edgeInElementCount[edge1] += 1;
				edgeInElementCount[edge2] += 1;
				edgeInElementCount[edge3] += 1;
				elementsThatContainNode[currElement->n1->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n2->nodeNumber].push_back(currElement);
				elementsThatContainNode[currElement->n3->nodeNumber].push_back(currElement);
			} else {
				std::cout << "WARNING: Unable to dereference element" << std::endl;
			}
		}

		/* Make a list of the boundary nodes */
		std::set<unsigned int> boundaryNodes;
		for (std::map<Edge, int>::iterator it = edgeInElementCount.begin(); it != edgeInElementCount.end(); ++it)
		{
			if (it->second == 1)
			{
				if (it->first.n1 != it->first.n2)
				{
					boundaryNodes.insert(it->first.n1);
					boundaryNodes.insert(it->first.n2);
				}
			}
		}

		/* Boundary elements are the ones that contain a boundary node */
		/* Go through the list of boundary elements and find the nodes of those elements
		 * that are not on the edge. These are the inner boundary nodes */
		for (std::set<unsigned int>::iterator it=boundaryNodes.begin(); it != boundaryNodes.end(); ++it)
		{
			unsigned int currentNode = *it;
			std::vector<Element*> elementsWithCurrNode = elementsThatContainNode[currentNode];
			if (elementsWithCurrNode.size() > 0)
			{
				for (std::vector<Element*>::iterator eIt=elementsWithCurrNode.begin(); eIt != elementsWithCurrNode.end(); ++eIt)
				{
					Element *currentElement = *eIt;
					if (currentElement)
					{
						if (boundaryNodes.count(currentElement->n1->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n1->nodeNumber);
						}
						if (boundaryNodes.count(currentElement->n2->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n2->nodeNumber);
						}
						if (boundaryNodes.count(currentElement->n3->nodeNumber) != 1)
						{
							innerBoundaryNodes.push_back(currentElement->n3->nodeNumber);
						}
					}
				}
			}
		}
	}

	if (innerBoundaryNodes.size() > 0)
	{
		std::sort(innerBoundaryNodes.begin(), innerBoundaryNodes.end());
		std::vector<unsigned int>::iterator it = std::unique(innerBoundaryNodes.begin(), innerBoundaryNodes.end());
		innerBoundaryNodes.resize(std::distance(innerBoundaryNodes.begin(), it));
	}

	return innerBoundaryNodes;
}


void BoundaryFinder::FindEdges(std::vector<Element *> *elements)
{
	for (std::vector<Element*>::iterator it = elements->begin(); it != elements->end(); ++it)
	{
		/* For each element, add the three edges to the list of edges */
		Edge newEdge1((*it)->n1->nodeNumber, (*it)->n2->nodeNumber);
		edgesMap[newEdge1] += 1;
		Edge newEdge2((*it)->n1->nodeNumber, (*it)->n3->nodeNumber);
		edgesMap[newEdge2] += 1;
		Edge newEdge3((*it)->n2->nodeNumber, (*it)->n3->nodeNumber);
		edgesMap[newEdge3] += 1;
	}

	for (std::map<Edge, int>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it)
	{
		if (it->second == 1)
		{
			nodeAdjacency[it->first.n1].push_back(it->first.n2);
			nodeAdjacency[it->first.n2].push_back(it->first.n1);
		}
	}
}


void BoundaryFinder::CreateEdgesList()
{
	if (nodeAdjacency.size() > 0)
	{
		unsigned int firstNode = nodeAdjacency.begin()->first;
		unsigned int secondNode = 0;
		while(true)
		{
			edgesList.push_back(firstNode);
			if (nodeAdjacency[firstNode][0] != 0)
			{
				secondNode = nodeAdjacency[firstNode][0];
				if (nodeAdjacency[secondNode][0] == firstNode)
					nodeAdjacency[secondNode][0] = 0;
				else
					nodeAdjacency[secondNode][1] = 0;
				nodeAdjacency[firstNode][0] = 0;
			}
			else if (nodeAdjacency[firstNode][1] != 0)
			{
				secondNode = nodeAdjacency[firstNode][1];
				if (nodeAdjacency[secondNode][0] == firstNode)
					nodeAdjacency[secondNode][0] = 0;
				else
					nodeAdjacency[secondNode][1] = 0;
				nodeAdjacency[firstNode][1] = 0;
			} else {
				break;
			}
			firstNode = secondNode;
		}
	}
}
