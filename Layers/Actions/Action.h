#ifndef ACTION_H
#define ACTION_H

// Prevent circular dependencies by defining the SelectionLayer class
class SelectionLayer;

/**
 * @brief The Action class is a virtual class that allows us to implement
 * undo and redo stacks in the SelectionLayer.
 *
 * The Action class is a virtual class that allows us to implement undo and
 * redo stacks in the SelectionLayer. A subclass of the Action class must
 * override the RedoAction and UndoAction functions. The RedoAction function
 * is called when the user clicks the redo button and the UndoAction function
 * is called when the user clicks the undo button.
 *
 * The SelectionLayer that is drawing the selected Nodes/Elements needs to be set
 * when an Action object is created. This is done by calling the SetSelectionLayer
 * function.
 *
 */
class Action
{
	public:
		Action();

		void		SetSelectionLayer(SelectionLayer* layer);
		virtual void	RedoAction() = 0;
		virtual void	UndoAction() = 0;

	protected:

		SelectionLayer*	selectionLayer;	/**< Pointer to the SelectionLayer object used to draw selected Nodes/Elements */
};

#endif // ACTION_H
