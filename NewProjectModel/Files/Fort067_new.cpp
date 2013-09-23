#include "Fort067_new.h"

Fort067_new::Fort067_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort067_new::Fort067_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort067_new::Fort067_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
