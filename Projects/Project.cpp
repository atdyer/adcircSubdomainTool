#include "Project.h"

Project::Project()
{
	fullDomain = 0;
}


Project::Project(QDir projectDir)
{
	Project();
	this->projectDir = projectDir;
}


Project::~Project()
{
	if (fullDomain)
		delete fullDomain;

	for (unsigned int i=0; i<subDomains.size(); ++i)
		if (subDomains[i])
			delete subDomains[i];
}


void Project::SetProjectDirectory(QDir newDirectory)
{

}


void Project::SetFullFort14(QDir newPath)
{

}


void Project::SetFullFort15(QDir newPath)
{

}


void Project::SetFullFort63(QDir newPath)
{

}
