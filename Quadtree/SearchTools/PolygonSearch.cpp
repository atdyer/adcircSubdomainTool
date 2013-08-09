#include "PolygonSearch.h"

PolygonSearch::PolygonSearch()
{

}


std::vector<Node*> PolygonSearch::FindNodes(branch *root, std::vector<Point> polyLine)
{
	fullNodes.clear();
	partialNodes.clear();
	polygonPoints = polyLine;

	SearchNodes(root);

	return fullNodes;
}


std::vector<Element*> PolygonSearch::FindElements(branch *root, std::vector<Point> polyLine)
{
	fullElements.clear();
	partialElements.clear();
	polygonPoints = polyLine;

	/* Build the two lists */
	SearchElements(root);

	/* Do a brute force search on the elements in partialElements */
	BruteForceElements();

	return fullElements;
}


void PolygonSearch::SearchNodes(branch *currBranch)
{

}


/**
 * @brief Recursively searches through the branch for Elements that may be part of the selection
 *
 * Recursively searches through the branch for Elements that may be part of the selection. Elements
 * that are guaranteed to fall inside of the polygon are added to the fullElements list. Elements that
 * could possibly fall inside of the polygon are added to the partialElements list.
 *
 * @param currBranch The branch through which recursion will take place
 */
void PolygonSearch::SearchElements(branch *currBranch)
{
	/* Search all branches */
	int branchCornersInsidePolygon = CountCornersInsidePolygon(currBranch);
	if (branchCornersInsidePolygon == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsidePolygon != 0 || (branchCornersInsidePolygon == 0 && (PolygonHasPointsInside(currBranch) || PolygonHasEdgeIntersection(currBranch))))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElements(currBranch->branches[i]);
			}
		}
	}

	/* Search all leaves */
	for (int i=0; i<4; ++i)
	{
		if (currBranch->leaves[i])
		{
			int leafCornersInsidePolygon = CountCornersInsidePolygon(currBranch->leaves[i]);
			if (leafCornersInsidePolygon == 4)
			{
				AddToFullElements(currBranch->leaves[i]);
			}
			else if (leafCornersInsidePolygon != 0)
			{
				AddToPartialElements(currBranch->leaves[i]);
			}
		}
	}

}


/**
 * @brief Determines if each individual Node in the partialNodes list falls within the polygon
 *
 * Determines if each individual Node in the partialNodes list falls within the polygon. If a
 * Node does fall within the polygon, it is added to the fullNodes list.
 *
 */
void PolygonSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsidePolygon(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


/**
 * @brief Determines if each individual Element in the partialElements list falls within the polygon
 *
 * Determines if each individual Element in the partialElements list falls within the polygon. If
 * an Element does fall within the polygon, it is added to the fullElements list.
 *
 */
void PolygonSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsidePolygon(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsidePolygon(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsidePolygon(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


/**
 * @brief Counts the number of corners of a branch that are inside of the polygon
 *
 * Counts the number of corners of a branch that are inside of the polygon. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currBranch Pointer to the branch to test
 * @return The number of corners of the branch that are inside of the polygon
 */
int PolygonSearch::CountCornersInsidePolygon(branch *currBranch)
{
	int count = 0;
	count += PointIsInsidePolygon(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsidePolygon(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Counts the number of corners of a leaf are inside of the polygon
 *
 * Counts the number of corners of a leaf that are inside of the polygon. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currLeaf Pointer to the leaf to test
 * @return The number of corners of the leaf that are inside of the polygon
 */
int PolygonSearch::CountCornersInsidePolygon(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsidePolygon(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsidePolygon(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Determines if any of the polygon points are inside of a branch
 *
 * Determines if any of the polygon points are inside of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the polygon has at least one point inside of the branch
 * @return false if the polygon has no points inside of the branch
 */
bool PolygonSearch::PolygonHasPointsInside(branch *currBranch)
{
	for (unsigned int i=0; i<polygonPoints.size(); ++i)
		if (PointIsInsideSquare(polygonPoints[i], currBranch))
			return true;
	return false;
}


/**
 * @brief Determines if any edge of the polygon intersects with any edge of a branch
 *
 * Determines if any edge of the polygon intersects with any edge of a branch.
 *
 * @param currBranch The branch to test
 * @return true if any edge of the polygon intersects with any edge of the branch
 * @return false if none of the edges of the polygon intersect with any edge of the branch
 */
bool PolygonSearch::PolygonHasEdgeIntersection(branch *currBranch)
{
	Point point1(currBranch->bounds[0], currBranch->bounds[2]);	/* Bottom Left */
	Point point2(currBranch->bounds[1], currBranch->bounds[2]);	/* Bottom Right */
	Point point3(currBranch->bounds[0], currBranch->bounds[3]);	/* Top Left */
	Point point4(currBranch->bounds[1], currBranch->bounds[3]);	/* Top Right */

	for (unsigned int i=0; i<polygonPoints.size()-1; ++i)
		if (EdgesIntersect(point1, point2, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point1, point3, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point3, point4, polygonPoints[i], polygonPoints[i+1]) ||
		    EdgesIntersect(point2, point4, polygonPoints[i], polygonPoints[i+1]))
			return true;
	return false;
}


bool PolygonSearch::PointIsInsidePolygon(float x, float y)
{
	return false;
}


bool PolygonSearch::PointIsInsideSquare(Point point, branch *currBranch)
{
	return false;
}


bool PolygonSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	return false;
}


bool PolygonSearch::EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD)
{

}


/**
 * @brief Adds all Nodes below the branch to the fullNodes list
 *
 * Adds all Nodes below the branch to the fullNodes list.
 *
 * @param currBranch The branch below which all Nodes will be added
 */
void PolygonSearch::AddToFullNodes(branch *currBranch)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			AddToFullNodes(currBranch->branches[i]);
		}
		if (currBranch->leaves[i])
		{
			AddToFullNodes(currBranch->leaves[i]);
		}
	}
}


/**
 * @brief Adds all Nodes in the leaf to the fullNodes list
 *
 * Adds all Nodes in the leaf to the fullNodes list.
 *
 * @param currLeaf The leaf whose Nodes will be added
 */
void PolygonSearch::AddToFullNodes(leaf *currLeaf)
{
	if (currLeaf->nodes.size() > 0)
		fullNodes.insert(fullNodes.end(), currLeaf->nodes.begin(), currLeaf->nodes.end());
}


/**
 * @brief Adds all Nodes in the leaf to the partialNodes list
 *
 * Adds all Nodes in the leaf to the partialNodes list.
 *
 * @param currLeaf The leaf whose Nodes will be added
 */
void PolygonSearch::AddToPartialNodes(leaf *currLeaf)
{
	if (currLeaf->nodes.size() > 0)
		partialNodes.insert(partialNodes.end(), currLeaf->nodes.begin(), currLeaf->nodes.end());
}


/**
 * @brief Adds all Elements below the branch to the fullElements list
 *
 * Adds all Elements below the branch to the fullElements list.
 *
 * @param currBranch The branch below which all Elements will be added
 */
void PolygonSearch::AddToFullElements(branch *currBranch)
{
	for (int i=0; i<4; ++i)
	{
		if (currBranch->branches[i])
		{
			AddToFullElements(currBranch->branches[i]);
		}
		if (currBranch->leaves[i])
		{
			AddToFullElements(currBranch->leaves[i]);
		}
	}
}


/**
 * @brief Adds all Elements in the leaf to the fullElements list
 *
 * Adds all Elements in the leaf to the fullElements list.
 *
 * @param currLeaf The leaf whose Elements will be added
 */
void PolygonSearch::AddToFullElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		fullElements.insert(fullElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}


/**
 * @brief Adds all Elements in the leaf to the partialElements list
 *
 * Adds all Elements in the leaf to the partialElements list
 *
 * @param currLeaf The leaf whose Elements will be added
 */
void PolygonSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
