#ifndef ELEMENTACTION_H
#define ELEMENTACTION_H

#include "Action.h"


/**
 * @brief A subclass of Action that represents the selection/deselection of
 * Elements in the SelectionLayer
 */
class ElementAction : public Action
{
	public:
		ElementAction();

		virtual void	RedoAction();
		virtual void	UndoAction();
};

#endif // ELEMENTACTION_H
