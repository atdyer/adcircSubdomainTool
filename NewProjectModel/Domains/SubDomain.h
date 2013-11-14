#ifndef SUBDOMAIN_H
#define SUBDOMAIN_H

#include <iostream>

#include <QObject>
#include <QProgressBar>

#include "NewProjectModel/Domains/Domain_new.h"

#include "NewProjectModel/Files/BNList14_new.h"
#include "NewProjectModel/Files/Fort14_new.h"
#include "NewProjectModel/Files/Fort15_new.h"
#include "NewProjectModel/Files/Fort22_new.h"
#include "NewProjectModel/Files/Fort63_new.h"
#include "NewProjectModel/Files/Fort64_new.h"
#include "NewProjectModel/Files/Fort015_new.h"
#include "NewProjectModel/Files/Fort020_new.h"
#include "NewProjectModel/Files/Fort021_new.h"
#include "NewProjectModel/Files/Fort022_new.h"
#include "NewProjectModel/Files/Maxele63_new.h"
#include "NewProjectModel/Files/Maxvel63_new.h"
#include "NewProjectModel/Files/ProjectFile_new.h"
#include "NewProjectModel/Files/Py140_new.h"
#include "NewProjectModel/Files/Py141_new.h"

#include "OpenGL/GLCamera.h"

class SubDomain : public Domain_new
{
	public:
		SubDomain(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);
		~SubDomain();

		virtual bool	IsFullDomain();

		Fort015_new*	GetFort015();
		QString		GetDomainName();

	private:

		BNList14_new*		bnList;
		QString			domainName;
		Fort15_new*		fort15;
		Fort22_new*		fort22;
		Fort63_new*		fort63;
		Fort64_new*		fort64;
		Fort015_new*		fort015;
		Fort020_new*		fort020;
		Fort021_new*		fort021;
		Fort022_new*		fort022;
		Maxele63_new*		maxele;
		Maxvel63_new*		maxvel;
		Py140_new*		py140;
		Py141_new*		py141;

		void	CreateAllFiles();


};

#endif // SUBDOMAIN_H
