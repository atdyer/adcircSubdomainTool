#include "ElementAction.h"

ElementAction::ElementAction()
{

}


/**
 * @brief Constructor used to create a new Action from a map of Elements
 *
 * This constructor should be used when creating a new ElementAction. It
 * stores the map of Elements that will be used when performing actions.
 *
 * @param cElements The map of Elements used to perform actions
 */
ElementAction::ElementAction(std::map<unsigned int, Element *> cElements)
{
	elements = cElements;
}


/**
 * @brief Selects all Elements
 *
 * If the SelectionLayer has been set and the map of Elements is not empty,
 * this function adds all of the Elements in the map to the list of Elements
 * to be drawn by the SelectionLayer
 *
 */
void ElementAction::RedoAction()
{
	if (selectionLayer && elements.size() > 0)
	{
		selectionLayer->SelectElements(elements);
	}
}


/**
 * @brief Deselects all Elements
 *
 * If the SelectionLayer has been set and the map of Elements is not empty,
 * this function removes all of the Elements in the map from the list of
 * Elements to be drawn by the SelectionLayer
 *
 */
void ElementAction::UndoAction()
{
	if (selectionLayer && elements.size() > 0)
	{
		selectionLayer->DeselectElements(elements);
	}
}
