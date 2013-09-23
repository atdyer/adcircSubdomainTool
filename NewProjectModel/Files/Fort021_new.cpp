#include "Fort021_new.h"

Fort021_new::Fort021_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort021_new::Fort021_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
