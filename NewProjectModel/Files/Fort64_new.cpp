#include "Fort64_new.h"

Fort64_new::Fort64_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort64_new::Fort64_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort64_new::Fort64_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}

