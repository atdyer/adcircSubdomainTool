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

}


void CircleSearch::BruteForceElements()
{

}


int CircleSearch::CountCornersInsideCircle(branch *currBranch)
{
	return 0;
}


int CircleSearch::CountCornersInsideCircle(leaf *currLeaf)
{
	return 0;
}


bool CircleSearch::PointIsInsideCircle(float x, float y)
{
	return false;
}


bool CircleSearch::PointIsInsideSquare(Point point, branch *currBranch)
{
	return false;
}


bool CircleSearch::PointIsInsideSquare(Point point, leaf *currLeaf)
{
	return false;
}


bool CircleSearch::CircleIsInsideSquare(branch *currBranch)
{
	return false;
}


bool CircleSearch::CircleIsInsideSquare(leaf *currLeaf)
{
	return false;
}


bool CircleSearch::CircleHasEdgeIntersection(branch *currBranch)
{
	return false;
}


bool CircleSearch::CircleHasEdgeIntersection(leaf *currLeaf)
{
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
