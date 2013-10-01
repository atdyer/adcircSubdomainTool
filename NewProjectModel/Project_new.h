#ifndef PROJECT_new_H
#define PROJECT_new_H


#include <QObject>
#include <QProgressBar>
#include <QTreeWidget>
#include <QInputDialog>

#include <vector>

#include "Adcirc/SubdomainCreator_new.h"

#include "Dialogs/CreateProjectDialog.h"

#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"

#include "NewProjectModel/Files/ProjectFile_new.h"



class Project_new : public QObject
{
		Q_OBJECT
	public:
		explicit Project_new(QObject *parent=0);
		Project_new(QString projectFile, QObject *parent=0);
		~Project_new();

		void	SetProgressBar(QProgressBar *newBar);
		void	SetProjectTree(QTreeWidget *newTree);

	private:

		FullDomain*		fullDomain;
		QProgressBar*		progressBar;
		ProjectFile_new*	projectFile;
		QTreeWidget*		projectTree;
		std::vector<SubDomain*>	subDomains;


		SubDomain*	BuildSubdomain(QString subdomainName);
		void		CreateAllSubdomains();
		void		CreateProjectFile();
		void		OpenProjectFile(QString filePath);
		void		PopulateProjectTree();

	public slots:

		void	CreateNewSubdomain();
		void	EditProjectSettings();
		void	RunFullDomain();
		void	RunSubdomain(QString subdomain);
		void	SaveProject();


};

#endif // PROJECT_new_H
