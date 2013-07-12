#include "NodeAction.h"

NodeAction::NodeAction()
{

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
