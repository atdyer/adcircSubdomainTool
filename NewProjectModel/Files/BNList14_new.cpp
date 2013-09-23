#include "BNList14_new.h"

BNList14_new::BNList14_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


BNList14_new::BNList14_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{
}


BNList14_new::BNList14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}
