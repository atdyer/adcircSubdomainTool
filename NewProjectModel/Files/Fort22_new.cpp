#include "Fort22_new.h"

Fort22_new::Fort22_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort22_new::Fort22_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort22_new::Fort22_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
