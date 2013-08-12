#ifndef CIRCLESEARCH_H
#define CIRCLESEARCH_H

#include <math.h>

#include "adcData.h"
#include "Quadtree/QuadtreeData.h"

/**
 * @brief A tool used to search a Quadtree for Nodes or Elements that fall within a circle
 */
class CircleSearch
{
	public:
		CircleSearch();

		std::vector<Node*>	FindNodes(branch *root, float x, float y, float radius);
		std::vector<Element*>	FindElements(branch *root, float x, float y, float radius);

	private:

		/* Circle Attributes */
		float	x;
		float	y;
		float	radius;

		/* Searching Lists */
		std::vector<Node*>	fullNodes;
		std::vector<Node*>	partialNodes;
		std::vector<Element*>	fullElements;
		std::vector<Element*>	partialElements;

		/* Search Functions */
		void	SearchNodes(branch *currBranch);
		void	SearchNodes(leaf *currLeaf);
		void	SearchElements(branch *currBranch);
		void	SearchElements(leaf *currLeaf);
		void	BruteForceNodes();
		void	BruteForceElements();

		/* Algorithm Functions */
		int	CountCornersInsideCircle(branch *currBranch);
		int	CountCornersInsideCircle(leaf *currLeaf);
		bool	CircleIsInsideSquare(branch *currBranch);
		bool	CircleIsInsideSquare(leaf *currLeaf);
		bool	CircleHasEdgeIntersection(branch *currBranch);
		bool	CircleHasEdgeIntersection(leaf *currLeaf);

		/* Helper Functions */
		bool	PointIsInsideCircle(float pointX, float pointY);
		bool	PointIsInsideSquare(Point point, branch *currBranch);
		bool	PointIsInsideSquare(Point point, leaf *currLeaf);
		bool	CircleIntersectsEdge(Point A, Point B);

		/* List Functions */
		void	AddToFullNodes(branch *currBranch);
		void	AddToFullNodes(leaf *currLeaf);
		void	AddToPartialNodes(leaf *currLeaf);
		void	AddToFullElements(branch *currBranch);
		void	AddToFullElements(leaf *currLeaf);
		void	AddToPartialElements(leaf *currLeaf);


};

#endif // CIRCLESEARCH_H
