#ifndef BOUNDARYCONDITIONSEXTRACTOR_H
#define BOUNDARYCONDITIONSEXTRACTOR_H

#include <vector>

#include <QString>

#include "NewProjectModel/Files/ProjectFile_new.h"
#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"

class BoundaryConditionsExtractor
{
	public:
		BoundaryConditionsExtractor();

		bool	ExtractBoundaryConditions(ProjectFile_new* file,
						  FullDomain* full,
						  std::vector<SubDomain*> subs);

	private:

		FullDomain*		fullDomain;
		ProjectFile_new*	projectFile;
		std::vector<SubDomain*>	subDomains;

		bool	AllVersionsCompatible();
		bool	CheckFullDomainFinished();
};

#endif // BOUNDARYCONDITIONSEXTRACTOR_H
