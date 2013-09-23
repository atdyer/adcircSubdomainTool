#include "Fort015_new.h"

Fort015_new::Fort015_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort015_new::Fort015_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort015_new::Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
