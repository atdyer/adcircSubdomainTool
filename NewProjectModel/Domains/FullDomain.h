#ifndef FULLDOMAIN_H
#define FULLDOMAIN_H

#include "NewProjectModel/Project_new.h"
#include "NewProjectModel/Domains/Domain_new.h"

class FullDomain : Domain_new
{
	public:
		FullDomain(Project_new *parentProject);

	private:

		Project_new*	parent;
};

#endif // FULLDOMAIN_H
