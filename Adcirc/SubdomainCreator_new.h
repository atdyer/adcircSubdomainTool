#ifndef SUBDOMAINCREATOR_NEW_H
#define SUBDOMAINCREATOR_NEW_H

#include <QString>
#include <QObject>

#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"
#include "NewProjectModel/Files/ProjectFile_new.h"

class SubdomainCreator_new
{
	public:
		SubdomainCreator_new();

		SubDomain*	CreateSubdomain(QString newName, ProjectFile_new *projectFile, FullDomain *fullDomain);
};

#endif // SUBDOMAINCREATOR_NEW_H
