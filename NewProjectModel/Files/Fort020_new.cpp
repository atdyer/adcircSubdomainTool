#include "Fort020_new.h"

Fort020_new::Fort020_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Fort020_new::Fort020_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
