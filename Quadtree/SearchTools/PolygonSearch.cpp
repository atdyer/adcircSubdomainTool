#include "PolygonSearch.h"

PolygonSearch::PolygonSearch()
{

}


std::vector<Node*> PolygonSearch::FindNodes(branch *root, std::vector<Point> polyLine)
{
	fullNodes.clear();
	partialNodes.clear();
	polygonPoints = polyLine;

	/* Perform search */

	return fullNodes;
}


std::vector<Element*> PolygonSearch::FindElements(branch *root, std::vector<Point> polyLine)
{
	fullElements.clear();
	partialElements.clear();

	/* Perform search */

	return fullElements;
}


void PolygonSearch::SearchNodes(branch *currBranch)
{

}


void PolygonSearch::SearchElements(branch *currBranch)
{

}


int PolygonSearch::CountCornersInsidePolygon(branch *currBranch)
{
	return 0;
}


int PolygonSearch::CountCornersInsidePolygon(leaf *currLeaf)
{
	return 0;
}


bool PolygonSearch::PolygonHasPointsInside(branch *currBranch)
{
	return false;
}


bool PolygonSearch::PolygonHasEdgeIntersection(branch *currBranch)
{
	return false;
}


bool PolygonSearch::PointIsInsidePolygon(float x, float y)
{
	return false;
}


bool PolygonSearch::PointIsInsideSquare(Point *point, branch *currBranch)
{
	return false;
}


bool PolygonSearch::PointIsInsideSquare(Point *point, leaf *currLeaf)
{
	return false;
}


void PolygonSearch::AddToFullNodes(branch *currBranch)
{

}


void PolygonSearch::AddToFullNodes(leaf *currLeaf)
{

}


void PolygonSearch::AddToPartialNodes(leaf *currLeaf)
{

}


void PolygonSearch::AddToFullElements(branch *currBranch)
{

}


void PolygonSearch::AddToFullElements(leaf *currLeaf)
{

}


void PolygonSearch::AddToPartialElements(leaf *currLeaf)
{

}
