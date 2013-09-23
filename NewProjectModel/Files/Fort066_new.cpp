#include "Fort066_new.h"

Fort066_new::Fort066_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort066_new::Fort066_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort066_new::Fort066_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
