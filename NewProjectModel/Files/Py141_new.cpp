#include "Py141_new.h"

Py141_new::Py141_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


Py141_new::Py141_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
