#ifndef PROJECT_new_H
#define PROJECT_new_H


#include <QObject>

#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"

#include "NewProjectModel/Files/ProjectFile_new.h"



class Project_new : QObject
{
		Q_OBJECT
	public:
		explicit Project_new(QObject *parent=0);
		~Project_new();

	private:

		ProjectFile_new*	projectFile;

		FullDomain*	fullDomain;
		SubDomain*	subDomain;
};

#endif // PROJECT_new_H
