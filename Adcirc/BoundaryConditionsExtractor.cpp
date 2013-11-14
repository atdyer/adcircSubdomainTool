#include "BoundaryConditionsExtractor.h"

BoundaryConditionsExtractor::BoundaryConditionsExtractor() :
	fullDomain(0),
	projectFile(0),
	subDomains()
{
}

bool BoundaryConditionsExtractor::ExtractBoundaryConditions(ProjectFile_new *file, FullDomain *full, std::vector<SubDomain *> subs)
{
	fullDomain = full;
	projectFile = file;
	subDomains = subs;
}


bool BoundaryConditionsExtractor::AllVersionsCompatible()
{
	if (fullDomain && subDomains.size())
	{
		Fort015_new *fullFort015 = fullDomain->GetFort015();
		if (fullFort015)
		{
			int fullVersion = fullFort015->GetSubdomainApproach();

			Fort015_new *currFort015;
			for (unsigned int i=0; i<subDomains.size(); ++i)
			{
				currFort015 = subDomains[i]->GetFort015();
				if (currFort015)
					if (currFort015->GetSubdomainApproach() != fullVersion)
						return false;

			}
			return true;
		}
	}
	return false;
}
