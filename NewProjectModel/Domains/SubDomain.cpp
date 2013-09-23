#include "SubDomain.h"


SubDomain::SubDomain(QObject *parent) :
	QObject(parent),
	bnList(0),
	domainName(),
	fort14(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort020(0),
	fort021(0),
	fort022(0),
	maxele(0),
	maxvel(0),
	projectFile(0),
	py140(0),
	py141(0)
{
	CreateAllFiles();
}


SubDomain::SubDomain(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	bnList(0),
	domainName(domainName),
	fort14(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort020(0),
	fort021(0),
	fort022(0),
	maxele(0),
	maxvel(0),
	projectFile(projectFile),
	py140(0),
	py141(0)
{

}


SubDomain::~SubDomain()
{
	std::cout << "Deleted subdomain: " << domainName.toStdString() << std::endl;
}


void SubDomain::CreateAllFiles()
{
	bnList = new BNList14_new(domainName, projectFile, this);
	fort14 = new Fort14_new(domainName, projectFile, this);
	fort15 = new Fort15_new(domainName, projectFile, this);
	fort22 = new Fort22_new(domainName, projectFile, this);
	fort63 = new Fort63_new(domainName, projectFile, this);
	fort64 = new Fort64_new(domainName, projectFile, this);
	fort015 = new Fort015_new(domainName, projectFile, this);
	fort020 = new Fort020_new(domainName, projectFile, this);
	fort021 = new Fort021_new(domainName, projectFile, this);
	fort022 = new Fort022_new(domainName, projectFile, this);
	maxele = new Maxele63_new(domainName, projectFile, this);
	maxvel = new Maxvel63_new(domainName, projectFile, this);
	py140 = new Py140_new(domainName, projectFile, this);
	py141 = new Py141_new(domainName, projectFile, this);
}
