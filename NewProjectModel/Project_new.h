#ifndef PROJECT_new_H
#define PROJECT_new_H


#include <QObject>
#include <QProgressBar>
#include <QTreeWidget>

#include <vector>

#include "Dialogs/CreateProjectDialog.h"

#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"

#include "NewProjectModel/Files/ProjectFile_new.h"



class Project_new : QObject
{
		Q_OBJECT
	public:
		explicit Project_new(QObject *parent=0);
		Project_new(QString projectFile, QObject *parent=0);
		~Project_new();

		void	SetProgressBar(QProgressBar *newBar);
		void	SetProjectTree(QTreeWidget *newTree);

	private:

		ProjectFile_new*	projectFile;

		FullDomain*		fullDomain;
		std::vector<SubDomain*>	subDomains;

		QProgressBar*		progressBar;
		QTreeWidget*		projectTree;

		void	CreateAllSubdomains();
		void	CreateProjectFile();
		void	OpenProjectFile(QString filePath);
		void	PopulateProjectTree();

};

#endif // PROJECT_new_H
