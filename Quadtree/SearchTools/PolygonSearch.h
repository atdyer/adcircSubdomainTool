#ifndef POLYGONSEARCH_H
#define POLYGONSEARCH_H

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A searching tool used to find Nodes or Elements from a Quadtree inside of a Polygon
 */
class PolygonSearch
{
	public:

		PolygonSearch();

		std::vector<Node*>	FindNodes(branch *root, std::vector<Point> polyLine);
		std::vector<Element*>	FindElements(branch *root, std::vector<Point> polyLine);

	private:

		std::vector<Point>	polygonPoints;
		std::vector<Node*>	fullNodes;
		std::vector<Node*>	partialNodes;
		std::vector<Element*>	fullElements;
		std::vector<Element*>	partialElements;

		/* Search Functions */
		void	SearchNodes(branch *currBranch);
		void	SearchElements(branch *currBranch);

		/* Algorithm Functions */
		int	CountCornersInsidePolygon(branch *currBranch);
		int	CountCornersInsidePolygon(leaf *currLeaf);

		bool	PolygonHasPointsInside(branch *currBranch);
		bool	PolygonHasEdgeIntersection(branch *currBranch);

		/* Helper Functions */
		bool	PointIsInsidePolygon(float x, float y);
		bool	PointIsInsideSquare(Point *point, branch *currBranch);
		bool	PointIsInsideSquare(Point *point, leaf *currLeaf);

		/* List Functions */
		void	AddToFullNodes(branch *currBranch);
		void	AddToFullNodes(leaf *currLeaf);
		void	AddToPartialNodes(leaf *currLeaf);
		void	AddToFullElements(branch *currBranch);
		void	AddToFullElements(leaf *currLeaf);
		void	AddToPartialElements(leaf *currLeaf);

};

#endif // POLYGONSEARCH_H
