#include "RectangleSearch.h"

RectangleSearch::RectangleSearch()
{
	l = 0.0;
	r = 0.0;
	b = 0.0;
	t = 0.0;
}


std::vector<Node*> RectangleSearch::FindNodes(branch *root, float l, float r, float b, float t)
{
	fullNodes.clear();
	partialNodes.clear();
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;

	SearchNodes(root);

	BruteForceNodes();

	return fullNodes;
}


std::vector<Element*> RectangleSearch::FindElements(branch *root, float l, float r, float b, float t)
{
	fullElements.clear();
	partialElements.clear();
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;

	SearchElements(root);

	BruteForceElements();

	return fullElements;
}


void RectangleSearch::SearchNodes(branch *currBranch)
{
	int branchCornersInsideRectange = CountCornersInsideRectangle(currBranch);
	if (branchCornersInsideRectange == 4)
	{
		AddToFullNodes(currBranch);
	}
	else if (branchCornersInsideRectange != 0 ||
		 RectangleHasEdgeIntersection(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchNodes(currBranch->branches[i]);
			}
			if (currBranch->leaves[i])
			{
				SearchNodes(currBranch->leaves[i]);
			}
		}
	}
}


void RectangleSearch::SearchNodes(leaf *currLeaf)
{
	int leafCornersInsideRectangle = CountCornersInsideRectangle(currLeaf);
	if (leafCornersInsideRectangle == 4)
	{
		AddToFullNodes(currLeaf);
	}
	else if (leafCornersInsideRectangle != 0 ||
		 RectangleHasEdgeIntersection(currLeaf))
	{
		AddToFullNodes(currLeaf);
	}
}


void RectangleSearch::SearchElements(branch *currBranch)
{
	int branchCornersInsideRectange = CountCornersInsideRectangle(currBranch);
	if (branchCornersInsideRectange == 4)
	{
		AddToFullElements(currBranch);
	}
	else if (branchCornersInsideRectange != 0 ||
		 RectangleHasEdgeIntersection(currBranch))
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


void RectangleSearch::SearchElements(leaf *currLeaf)
{
	int leafCornersInsideRectangle = CountCornersInsideRectangle(currLeaf);
	if (leafCornersInsideRectangle == 4)
	{
		AddToFullElements(currLeaf);
	}
	else if (leafCornersInsideRectangle != 0 ||
		 RectangleHasEdgeIntersection(currLeaf))
	{
		AddToPartialElements(currLeaf);
	}
}


void RectangleSearch::BruteForceNodes()
{
	Node *currNode = 0;
	for (std::vector<Node*>::iterator it = partialNodes.begin(); it != partialNodes.end(); ++it)
	{
		currNode = *it;
		if (PointIsInsideRectangle(currNode->normX, currNode->normY))
			fullNodes.push_back(currNode);
	}
}


void RectangleSearch::BruteForceElements()
{
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it = partialElements.begin(); it != partialElements.end(); ++it)
	{
		currElement = *it;
		if (PointIsInsideRectangle(currElement->n1->normX, currElement->n1->normY) ||
		    PointIsInsideRectangle(currElement->n2->normX, currElement->n2->normY) ||
		    PointIsInsideRectangle(currElement->n3->normX, currElement->n3->normY))
			fullElements.push_back(currElement);
	}
}


int RectangleSearch::CountCornersInsideRectangle(branch *currBranch)
{
	int count = 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


int RectangleSearch::CountCornersInsideRectangle(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


bool RectangleSearch::RectangleHasEdgeIntersection(branch *currBranch)
{
	return		!(l >= currBranch->bounds[1] ||
			  r <= currBranch->bounds[0] ||
			  b >= currBranch->bounds[3] ||
			  t <= currBranch->bounds[2]);
}


bool RectangleSearch::RectangleHasEdgeIntersection(leaf *currLeaf)
{
	return		!(l >= currLeaf->bounds[1] ||
			  r <= currLeaf->bounds[0] ||
			  b >= currLeaf->bounds[3] ||
			  t <= currLeaf->bounds[2]);
}


bool RectangleSearch::PointIsInsideRectangle(float x, float y)
{
	if (l <= x &&
	    x <= r &&
	    b <= y &&
	    y <= t)
		return true;
	return false;
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
bool RectangleSearch::PointIsInsideSquare(Point point, branch *currBranch)
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
bool RectangleSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	if (point.x >= currLeaf->bounds[0] &&
	    point.x <= currLeaf->bounds[1] &&
	    point.y >= currLeaf->bounds[2] &&
	    point.y <= currLeaf->bounds[3])
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
void RectangleSearch::AddToFullNodes(branch *currBranch)
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
void RectangleSearch::AddToFullNodes(leaf *currLeaf)
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
void RectangleSearch::AddToPartialNodes(leaf *currLeaf)
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
void RectangleSearch::AddToFullElements(branch *currBranch)
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
void RectangleSearch::AddToFullElements(leaf *currLeaf)
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
void RectangleSearch::AddToPartialElements(leaf *currLeaf)
{
	if (currLeaf->elements.size() > 0)
		partialElements.insert(partialElements.end(), currLeaf->elements.begin(), currLeaf->elements.end());
}
