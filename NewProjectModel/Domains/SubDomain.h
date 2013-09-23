#ifndef SUBDOMAIN_H
#define SUBDOMAIN_H

#include "NewProjectModel/Project_new.h"
#include "NewProjectModel/Domains/Domain_new.h"

class SubDomain : Domain_new
{
	public:
		SubDomain(Project_new *parentProject);

	private:

		Project_new*	parent;
};

#endif // SUBDOMAIN_H
