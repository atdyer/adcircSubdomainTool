#ifndef NODEACTION_H
#define NODEACTION_H

#include "adcData.h"
#include "Action.h"
#include "Layers/SelectionLayer.h"
#include <map>

class NodeAction : public Action
{
	public:

		NodeAction();
		NodeAction(std::map<unsigned int, Node*> cNodes);

		std::map<unsigned int, Node*>	nodes;

		virtual void	RedoAction();
		virtual void	UndoAction();
};

#endif // NODEACTION_H
