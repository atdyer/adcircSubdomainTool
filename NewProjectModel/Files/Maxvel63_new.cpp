#include "Maxvel63_new.h"

Maxvel63_new::Maxvel63_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Maxvel63_new::Maxvel63_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Maxvel63_new::Maxvel63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
