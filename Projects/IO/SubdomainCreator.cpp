#include "SubdomainCreator.h"

SubdomainCreator::SubdomainCreator()
{
}


void SubdomainCreator::CreateSubdomain()
{

}


void SubdomainCreator::SetDomain(Domain *newDomain)
{
	if (newDomain)
	{
		currentSelectedState = newDomain->GetCurrentSelectedElements();
		boundaryNodes = boundaryFinder.FindBoundaries(currentSelectedState);
	}
}


void SubdomainCreator::SetTargetPath(QString newTarget)
{
	QDir newPath (newTarget);
	if (newPath.exists())
	{
		targetPath = newTarget;
	}
}


void SubdomainCreator::WriteFort14File()
{

}
