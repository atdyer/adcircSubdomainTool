#include "Fort63_new.h"

Fort63_new::Fort63_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort63_new::Fort63_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort63_new::Fort63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
