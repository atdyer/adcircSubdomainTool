#include "FullDomain.h"


FullDomain::FullDomain(QObject *parent) :
	QObject(parent),
	fort14(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort066(0),
	fort067(0),
	maxele(0),
	maxvel(0),
	projectFile(0)
{

}


FullDomain::FullDomain(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	fort14(0),
	fort15(0),
	fort22(0),
	fort63(0),
	fort64(0),
	fort015(0),
	fort066(0),
	fort067(0),
	maxele(0),
	maxvel(0),
	projectFile(projectFile)
{
	CreateAllFiles();
}


FullDomain::~FullDomain()
{
	std::cout << "Deleted full domain" << std::endl;
}


std::vector<Element*> FullDomain::GetSelectedElements()
{

}


void FullDomain::SetProgressBar(QProgressBar *newBar)
{
	if (fort14)
		fort14->SetProgressBar(newBar);
}


void FullDomain::CreateAllFiles()
{
	fort14 = new Fort14_new(projectFile, this);
	fort15 = new Fort15_new(projectFile, this);
	fort22 = new Fort22_new(projectFile, this);
	fort63 = new Fort63_new(projectFile, this);
	fort64 = new Fort64_new(projectFile, this);
	fort015 = new Fort015_new(projectFile, this);
	fort066 = new Fort066_new(projectFile, this);
	fort067 = new Fort067_new(projectFile, this);
	maxele = new Maxele63_new(projectFile, this);
	maxvel = new Maxvel63_new(projectFile, this);
}
