#ifndef BOUNDARYFINDER_H
#define BOUNDARYFINDER_H

#include <vector>

#include <QObject>

#include "adcData.h"

class BoundaryFinder
{
		Q_OBJECT
	public:
		BoundaryFinder();

		std::vector<Node*>	FindBoundaries(std::vector<Node*> nodes, std::vector<Element*> elements);

};

#endif // BOUNDARYFINDER_H
