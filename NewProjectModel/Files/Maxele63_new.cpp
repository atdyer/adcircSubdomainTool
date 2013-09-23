#include "Maxele63_new.h"

Maxele63_new::Maxele63_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Maxele63_new::Maxele63_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Maxele63_new::Maxele63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
