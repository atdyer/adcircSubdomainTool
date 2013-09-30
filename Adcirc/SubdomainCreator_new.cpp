#include "SubdomainCreator_new.h"

SubdomainCreator_new::SubdomainCreator_new()
{
}


SubDomain* SubdomainCreator_new::CreateSubdomain(QString newName, ProjectFile_new *projectFile, FullDomain *fullDomain)
{
	if (projectFile && fullDomain)
	{
		if (projectFile->AddSubdomain(newName))
		{
			// Create files and whatnot

			// Create the class
			SubDomain *newSubdomain = new SubDomain(newName, projectFile);
			return newSubdomain;
		}
	}
	return 0;
}
