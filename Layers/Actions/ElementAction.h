#ifndef ELEMENTACTION_H
#define ELEMENTACTION_H

#include "Action.h"

class ElementAction : public Action
{
	public:
		ElementAction();

		virtual void	RedoAction();
		virtual void	UndoAction();
};

#endif // ELEMENTACTION_H
