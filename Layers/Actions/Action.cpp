#include "Action.h"

Action::Action()
{
}


/**
 * @brief Sets the SelectionLayer object that the Action is performed on
 *
 * Sets the SelectionLayer object that the Action is performed on. This is the
 * Layer that we give a list of Nodes or Elements to either add or remove to
 * the selected Nodes/Elements list.
 *
 * @param layer Pointer to the SelectionLayer
 */
void Action::SetSelectionLayer(SelectionLayer *layer)
{
	selectionLayer = layer;
}
