#ifndef BOUNDARYFINDER_H
#define BOUNDARYFINDER_H

#include <vector>
#include <set>

#include <QObject>

#include "adcData.h"

class BoundaryFinder : public QObject
{
		Q_OBJECT
	public:
		BoundaryFinder();

		std::vector<Node*>	FindBoundaries(std::vector<Element *> fullElements, std::vector<Element*> selectedElements);

};

#endif // BOUNDARYFINDER_H
