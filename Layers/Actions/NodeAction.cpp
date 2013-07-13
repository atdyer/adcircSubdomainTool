#include "NodeAction.h"

NodeAction::NodeAction()
{

}


NodeAction::NodeAction(std::map<unsigned int, Node *> cNodes)
{
	nodes = cNodes;
}


void NodeAction::RedoAction()
{
	if (selectionLayer && nodes.size() > 0)
	{
		selectionLayer->SelectNodes(nodes);
	}
}


void NodeAction::UndoAction()
{
	if (selectionLayer && nodes.size() > 0)
	{
		selectionLayer->DeselectNodes(nodes);
	}
}
