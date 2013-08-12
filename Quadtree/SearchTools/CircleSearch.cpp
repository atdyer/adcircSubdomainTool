#include "CircleSearch.h"

CircleSearch::CircleSearch()
{
	x = 0.0;
	y = 0.0;
	radius = 0.0;
}


std::vector<Node*> CircleSearch::FindNodes(branch *root, float x, float y, float radius)
{
	return fullNodes;
}


std::vector<Element*> CircleSearch::FindElements(branch *root, float x, float y, float radius)
{
	fullElements.clear();
	partialElements.clear();
	this->x = x;
	this->y = y;
	this->radius = radius;

	/* Build the two lists */
	SearchElements(root);

	/* Do a brute force search on the elements in partialElements */
	BruteForceElements();

	return fullElements;
}


void CircleSearch::SearchNodes(branch *currBranch)
{

}


void CircleSearch::SearchNodes(leaf *currLeaf)
{

}


void CircleSearch::SearchElements(branch *currBranch)
{
	int branchCornersInsideCircle = CountCornersInsideCircle(currBranch);
	if (branchCornersInsideCircle == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currBranch) ||
		 CircleHasEdgeIntersection(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElements(currBranch->branches[i]);
			}
			if (currBranch->leaves[i])
			{
				SearchElements(currBranch->leaves[i]);
			}
		}
	}
}


void CircleSearch::SearchElements(leaf *currLeaf)
{
	int leafCornersInsideCircle = CountCornersInsideCircle(currLeaf);
	if (leafCornersInsideCircle == 4)
	{
		AddToFullElements(currLeaf);
	}
	else if (leafCornersInsideCircle != 0 ||
		 CircleIsInsideSquare(currLeaf) ||
		 CircleHasEdgeIntersection(currLeaf))
	{
		AddToPartialElements(currLeaf);
	}
}


void CircleSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsideCircle(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


void CircleSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsideCircle(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsideCircle(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsideCircle(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


int CircleSearch::CountCornersInsideCircle(branch *currBranch)
{
	int count = 0;
	count += PointIsInsideCircle(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsideCircle(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


int CircleSearch::CountCornersInsideCircle(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsideCircle(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsideCircle(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


bool CircleSearch::CircleIsInsideSquare(branch *currBranch)
{
	if (PointIsInsideSquare(Point(x-radius, y), currBranch) &&
	    PointIsInsideSquare(Point(x+radius, y), currBranch) &&
	    PointIsInsideSquare(Point(x, y-radius), currBranch) &&
	    PointIsInsideSquare(Point(x, y+radius), currBranch))
		return true;
	return false;
}


bool CircleSearch::CircleIsInsideSquare(leaf *currLeaf)
{
	if (PointIsInsideSquare(Point(x-radius, y), currLeaf) &&
	    PointIsInsideSquare(Point(x+radius, y), currLeaf) &&
	    PointIsInsideSquare(Point(x, y-radius), currLeaf) &&
	    PointIsInsideSquare(Point(x, y+radius), currLeaf))
		return true;
	return false;
}


bool CircleSearch::CircleHasEdgeIntersection(branch *currBranch)
{
	Point BottomLeft(currBranch->bounds[0], currBranch->bounds[2]);
	Point BottomRight(currBranch->bounds[1], currBranch->bounds[2]);
	Point TopLeft(currBranch->bounds[0], currBranch->bounds[3]);
	Point TopRight(currBranch->bounds[1], currBranch->bounds[3]);

	if (CircleIntersectsEdge(BottomLeft, BottomRight) ||
	    CircleIntersectsEdge(BottomRight, TopRight) ||
	    CircleIntersectsEdge(TopRight, TopLeft) ||
	    CircleIntersectsEdge(TopLeft, BottomLeft))
		return true;

	return false;
}


bool CircleSearch::CircleHasEdgeIntersection(leaf *currLeaf)
{
	Point BottomLeft(currLeaf->bounds[0], currLeaf->bounds[2]);
	Point BottomRight(currLeaf->bounds[1], currLeaf->bounds[2]);
	Point TopLeft(currLeaf->bounds[0], currLeaf->bounds[3]);
	Point TopRight(currLeaf->bounds[1], currLeaf->bounds[3]);

	if (CircleIntersectsEdge(BottomLeft, BottomRight) ||
	    CircleIntersectsEdge(BottomRight, TopRight) ||
	    CircleIntersectsEdge(TopRight, TopLeft) ||
	    CircleIntersectsEdge(TopLeft, BottomLeft))
		return true;

	return false;
}


bool CircleSearch::PointIsInsideCircle(float pointX, float pointY)
{
	return (pow(pointX - x, 2.0) + (pow(pointY - y, 2.0))) < pow(radius, 2.0);
}


/**
 * @brief Determines if a point is inside the bounding box of a branch
 *
 * Determines if a point is inside the bounding box of a branch.
 *
 * @param point The point to test
 * @param currBranch The branch to test
 * @return true if the point falls within the bounds of the branch
 * @return false if the point does not fall within the bounds of the branch
 */
bool CircleSearch::PointIsInsideSquare(Point point, branch *currBranch)
{
	if (point.x >= currBranch->bounds[0] &&
	    point.x <= currBranch->bounds[1] &&
	    point.y >= currBranch->bounds[2] &&
	    point.y <= currBranch->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if a point is inside the bounding box of a leaf
 *
 * Determines if a point is inside the bounding box of a leaf.
 *
 * @param point The point to test
 * @param currLeaf The leaf to test
 * @return true if the point falls within the bounds of the leaf
 * @return false if the point does not fall within the bounds of the leaf
 */
bool CircleSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	if (point.x >= currLeaf->bounds[0] &&
	    point.x <= currLeaf->bounds[1] &&
	    point.y >= currLeaf->bounds[2] &&
	    point.y <= currLeaf->bounds[3])
		return true;
	return false;
}


bool CircleSearch::CircleIntersectsEdge(Point A, Point B)
{
	float left = A.x < B.x ? A.x : B.x;
	float right = A.x < B.x ? B.x : A.x;
	float bottom = A.y < B.y ? A.y : B.y;
	float top = A.y < B.y ? B.y : A.y;

	if (left-radius < x &&
	    x < right + radius &&
	    bottom-radius < y &&
	    y < top + radius)
		return true;

	return false;
}


/**
 * @brief Adds all Nodes below the branch to the fullNodes list
 *
 * Adds all Nodes below the branch to the fullNodes list.
 *
 * @param currBranch The branch below which all Nodes will be added
 */
void CircleSearch::AddToFullNodes(branch *currBranch)
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
void CircleSearch::AddToFullNodes(leaf *currLeaf)
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
void CircleSearch::AddToPartialNodes(leaf *currLeaf)
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
void CircleSearch::AddToFullElements(branch *currBranch)
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
void CircleSearch::AddToFullElements(leaf *currLeaf)
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
void CircleSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
