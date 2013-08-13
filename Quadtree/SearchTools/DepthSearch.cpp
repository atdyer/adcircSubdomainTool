#include "DepthSearch.h"

DepthSearch::DepthSearch()
{

}


std::vector< std::vector<Element*> *> DepthSearch::FindElements(branch *root, int depth)
{
	listOfElementLists.clear();

	RetrieveElements(root, depth);

	return listOfElementLists;
}


std::vector< std::vector<Element*> *> DepthSearch::FindElements(branch *root, int depth, float l, float r, float b, float t)
{
	this->l = l;
	this->r = r;
	this->b = b;
	this->t = t;

	listOfElementLists.clear();

	SearchElements(root, depth);

	return listOfElementLists;

}


void DepthSearch::SearchElements(branch *currBranch, int depth)
{
	int branchCornersInsideRectangle = CountCornersInsideRectangle(currBranch);
	if (branchCornersInsideRectangle == 4)
	{
		RetrieveElements(currBranch, depth);
	}
	else if (branchCornersInsideRectangle != 0 ||
		 RectangleHasIntersection(currBranch))
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				SearchElements(currBranch->branches[i], depth);
			}
			if (currBranch->leaves[i])
			{
				SearchElements(currBranch->leaves[i]);
			}
		}
	}
}


void DepthSearch::SearchElements(leaf *currLeaf)
{
	if (RectangleHasIntersection(currLeaf))
	{
		AddToListOfElements(currLeaf);
	}
}


void DepthSearch::RetrieveElements(branch *currBranch, int depth)
{
	if (depth > 0)
	{
		for (int i=0; i<4; ++i)
		{
			if (currBranch->branches[i])
			{
				RetrieveElements(currBranch->branches[i], depth-1);
			}
			if (currBranch->leaves[i])
			{
				AddToListOfElements(currBranch->leaves[i]);
			}
		}
	}
}


/**
 * @brief Counts the number of corners of a branch that are inside of the rectangle
 *
 * Counts the number of corners of a branch that are inside of the rectangle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currBranch Pointer to the branch to test
 * @return The number of corners of the branch that are inside of the rectangle
 */
int DepthSearch::CountCornersInsideRectangle(branch *currBranch)
{
	int count = 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[0], currBranch->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currBranch->bounds[1], currBranch->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Counts the number of corners of a leaf that are inside of the rectangle
 *
 * Counts the number of corners of a leaf that are inside of the rectangle. Will always
 * return a number between (and including) 0 and 4
 *
 * @param currLeaf Pointer to the leaf to test
 * @return The number of corners of the leaf that are inside of the rectangle
 */
int DepthSearch::CountCornersInsideRectangle(leaf *currLeaf)
{
	int count = 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[2]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[0], currLeaf->bounds[3]) ? 1 : 0;
	count += PointIsInsideRectangle(currLeaf->bounds[1], currLeaf->bounds[3]) ? 1 : 0;
	return count;
}


/**
 * @brief Determines if the rectangle has an intersection with any edge of a branch
 *
 * Determines if the rectangle has an intersection with any edge of a branch.
 *
 * @param currBranch The branch to test
 * @return true if the rectangle intersects any edge of the branch
 * @return false if the rectangle does not intersect any edge of the branch
 */
bool DepthSearch::RectangleHasIntersection(branch *currBranch)
{
	return		!(l >= currBranch->bounds[1] ||
			  r <= currBranch->bounds[0] ||
			  b >= currBranch->bounds[3] ||
			  t <= currBranch->bounds[2]);
}


/**
 * @brief Determines if the rectangle has an intersection with any edge of a leaf
 *
 * Determines if the rectangle has an intersection with any edge of a leaf.
 *
 * @param currLeaf The leaf to test
 * @return true if the rectangle intersects any edge of the leaf
 * @return false if the rectangle does not intersect any edge of the leaf
 */
bool DepthSearch::RectangleHasIntersection(leaf *currLeaf)
{
	return		!(l >= currLeaf->bounds[1] ||
			  r <= currLeaf->bounds[0] ||
			  b >= currLeaf->bounds[3] ||
			  t <= currLeaf->bounds[2]);
}


/**
 * @brief Determines if a point is inside of the rectangle
 *
 * Determines if a point is inside of the rectangle.
 *
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @return true if the point falls inside of the rectangle
 * @return false if the point does not fall inside of the rectangle
 */
bool DepthSearch::PointIsInsideRectangle(float x, float y)
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
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param currBranch The branch to test
 * @return true if the point falls within the bounds of the branch
 * @return false if the point does not fall within the bounds of the branch
 */
bool DepthSearch::PointIsInsideSquare(float x, float y, branch *currBranch)
{
	if (x >= currBranch->bounds[0] &&
	    x <= currBranch->bounds[1] &&
	    y >= currBranch->bounds[2] &&
	    y <= currBranch->bounds[3])
		return true;
	return false;
}


/**
 * @brief Determines if a point is inside the bounding box of a leaf
 *
 * Determines if a point is inside the bounding box of a leaf.
 *
 * @param x The x-coordinate of the point
 * @param y The y-coordinate of the point
 * @param currLeaf The leaf to test
 * @return true if the point falls within the bounds of the leaf
 * @return false if the point does not fall within the bounds of the leaf
 */
bool DepthSearch::PointIsInsideSquare(float x, float y, leaf *currLeaf)
{
	if (x >= currLeaf->bounds[0] &&
	    x <= currLeaf->bounds[1] &&
	    y >= currLeaf->bounds[2] &&
	    y <= currLeaf->bounds[3])
		return true;
	return false;
}


void DepthSearch::AddToListOfElements(leaf *currLeaf)
{
	listOfElementLists.push_back(&currLeaf->elements);
}
