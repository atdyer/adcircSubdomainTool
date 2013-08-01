#ifndef BOUNDARYFINDER_H
#define BOUNDARYFINDER_H

#include <vector>
#include <map>

#include "adcData.h"
#include "Layers/Actions/ElementState.h"

struct Edge
{
		unsigned int n1;
		unsigned int n2;
		bool operator== (const Edge &e1) const{
			return (n1 == e1.n1 && n2 == e1.n2);
		}
		bool operator< (const Edge &e1) const{
			return n1 < e1.n1;
		}
};

class BoundaryFinder
{
	public:
		/* Constructor */
		BoundaryFinder();

		/* The Callable Search Function */
		std::vector<Node*>	FindBoundaries(ElementState* elementSelection);

	private:

		std::map<Edge, int>	edgesMap;

		void	CreateEdgesList(std::vector<Element*>* elements);

};

#endif // BOUNDARYFINDER_H
