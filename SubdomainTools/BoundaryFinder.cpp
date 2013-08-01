#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder()
{
}


std::vector<Node*> BoundaryFinder::FindBoundaries(ElementState *elementSelection)
{
	CreateEdgesList(elementSelection->GetState());
	std::vector<Node*> list;
	return list;
}


void BoundaryFinder::CreateEdgesList(std::vector<Element *> *elements)
{
	/********** NOT WORKING YET!! **************/
	edgesMap.clear();
	for (std::vector<Element*>::iterator it = elements->begin(); it != elements->end(); ++it)
	{
		Edge newEdge {(*it)->n1->nodeNumber, (*it)->n2->nodeNumber};
		edgesMap[newEdge] += 1;
	}

	DEBUG("Num edges: " << edgesMap.size());

	int boundaryEdges = 0;
	for (std::map<Edge, int>::iterator it = edgesMap.begin(); it != edgesMap.end(); ++it)
		if (it->second == 1)
			boundaryEdges++;
	DEBUG("Num boundary edges: " << boundaryEdges);
}
