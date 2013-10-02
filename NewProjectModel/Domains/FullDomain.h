#ifndef FULLDOMAIN_H
#define FULLDOMAIN_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QProgressBar>

#include "NewProjectModel/Files/Fort14_new.h"
#include "NewProjectModel/Files/Fort15_new.h"
#include "NewProjectModel/Files/Fort22_new.h"
#include "NewProjectModel/Files/Fort63_new.h"
#include "NewProjectModel/Files/Fort64_new.h"
#include "NewProjectModel/Files/Fort015_new.h"
#include "NewProjectModel/Files/Fort066_new.h"
#include "NewProjectModel/Files/Fort067_new.h"
#include "NewProjectModel/Files/Maxele63_new.h"
#include "NewProjectModel/Files/Maxvel63_new.h"
#include "NewProjectModel/Files/ProjectFile_new.h"


class FullDomain : public QObject
{
		Q_OBJECT
	public:
		explicit FullDomain(QObject *parent=0);
		FullDomain(ProjectFile_new *projectFile, QObject *parent=0);
		~FullDomain();

		std::vector<Element*>	GetSelectedElements();

		void			SetProgressBar(QProgressBar *newBar);

	private:

		Fort14_new*		fort14;
		Fort15_new*		fort15;
		Fort22_new*		fort22;
		Fort63_new*		fort63;
		Fort64_new*		fort64;
		Fort015_new*		fort015;
		Fort066_new*		fort066;
		Fort067_new*		fort067;
		Maxele63_new*		maxele;
		Maxvel63_new*		maxvel;
		ProjectFile_new*	projectFile;


		void	CreateAllFiles();

};

#endif // FULLDOMAIN_H
