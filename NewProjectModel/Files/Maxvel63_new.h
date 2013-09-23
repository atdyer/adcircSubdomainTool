#ifndef MAXVEL63_new_H
#define MAXVEL63_new_H

#include "NewProjectModel/Domains/Domain_new.h"
#include "NewProjectModel/ProjectSettings_new.h"

class Maxvel63_new
{
	public:
		Maxvel63_new(Domain_new *parent);

	private:
		ProjectSettings_new*	projectSettings;
};

#endif // MAXVEL63_new_H
