#ifndef POLYGONSEARCH_H
#define POLYGONSEARCH_H

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A tool used to search a Quadtree for Nodes or Elements that fall within a polygon
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
		void	BruteForceNodes();
		void	BruteForceElements();

		/* Algorithm Functions */
		int	CountCornersInsidePolygon(branch *currBranch);
		int	CountCornersInsidePolygon(leaf *currLeaf);

		bool	PolygonHasPointsInside(branch *currBranch);
		bool	PolygonHasPointsInside(leaf *currLeaf);
		bool	PolygonHasEdgeIntersection(branch *currBranch);
		bool	PolygonHasEdgeIntersection(leaf* currLeaf);

		/* Helper Functions */
		bool	PointIsInsidePolygon(float x, float y);
		bool	PointIsInsideSquare(Point point, branch *currBranch);
		bool	PointIsInsideSquare(Point point, leaf *currLeaf);
		bool	EdgesIntersect(Point pointA, Point pointB, Point pointC, Point pointD);

		/* List Functions */
		void	AddToFullNodes(branch *currBranch);
		void	AddToFullNodes(leaf *currLeaf);
		void	AddToPartialNodes(leaf *currLeaf);
		void	AddToFullElements(branch *currBranch);
		void	AddToFullElements(leaf *currLeaf);
		void	AddToPartialElements(leaf *currLeaf);

};

#endif // POLYGONSEARCH_H
