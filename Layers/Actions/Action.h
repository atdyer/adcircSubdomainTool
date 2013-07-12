#ifndef ACTION_H
#define ACTION_H

// Prevent circular dependencies by defining the SelectionLayer class
class SelectionLayer;

/**
 * @brief The Action class is an abstract class that allows us to implement
 * undo and redo stacks in the SelectionLayer.
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

		SelectionLayer*	selectionLayer;
};

#endif // ACTION_H
