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
