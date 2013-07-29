#include "BoundaryFinder.h"

BoundaryFinder::BoundaryFinder()
{
}


std::vector<Node*> BoundaryFinder::FindBoundaries(std::vector<Element *> fullElements, std::vector<Element *> selectedElements)
{
//	// A set that contains all selected nodes
//	std::set<Node*> selectedNodes;

//	// Add all nodes from the selected elements to the selected nodes
//	for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); it++)
//	{
//		selectedNodes.insert(*it);
//	}

//	// A list of the boundary nodes
//	std::vector<Node*> boundaryNodes;

//	// Find our first boundary node using brute force
//	Element* currElement = 0;
//	for (std::vector<Element*>::iterator it = fullElements.begin(); it != fullElements.end(); it++)
//	{
//		bool n1 = false;
//		bool n2 = false;
//		bool n3 = false;
//		currElement = *it;
//		if (selectedNodes.count(currElement->n1))
//			n1 = true;
//		if (selectedNodes.count(currElement->n2))
//			n2 = true;
//		if (selectedNodes.count(currElement->n3))
//			n3 = true;

//		// If they aren't all true or they aren't all false, we've found a boundary node
//		if (!(n1 && n2 && n3) || !(!n1 && !n2 && !n3))
//			break;
//	}

//	// Add our first boundary node
//	boundaryNodes.push_back();

}
