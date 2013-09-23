#include "Fort022_new.h"

Fort022_new::Fort022_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort022_new::Fort022_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


Fort022_new::Fort022_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
