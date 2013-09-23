#include "Py140_new.h"

Py140_new::Py140_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Py140_new::Py140_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Py140_new::Py140_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
