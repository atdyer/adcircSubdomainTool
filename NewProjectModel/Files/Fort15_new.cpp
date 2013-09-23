#include "Fort15_new.h"

Fort15_new::Fort15_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort15_new::Fort15_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort15_new::Fort15_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
