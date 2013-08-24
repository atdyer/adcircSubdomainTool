#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QObject>
#include <QDir>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QTextStream>
#include <QTreeWidget>

#include "Dialogs/CreateProjectDialog.h"

#include "Domains/Domain.h"


/**
 * @brief This class represents an ADCIRC Subdomain Project
 *
 * This class represents an ADCIRC Subdomain Project. An ADCIRC Subdomain Project consists of a single full domain
 * and one or more subdomains.
 *
 */
class Project : public QObject
{
		Q_OBJECT
	public:
		Project();
		~Project();

		/* Project-wide functionality */
		void	SetProjectTree(QTreeWidget *newTree);

		/* Creating a new project */
		bool	CreateProject();
		bool	CreateProject(QString directory, QString projName);

		/* Opening a project */
		bool	OpenProject();

		/* Editing a project */
		void	SetFullFort14(QString newPath);
		void	SetFullFort15(QString newPath);
		void	SetFullFort63(QString newPath);

		unsigned int	CreateNewSubdomain(QString newName);

		/* Active domain fetching functions */
		Domain*		GetFullDomain();
		Domain*		GetActiveSubdomain();

	private:

		QTreeWidget*	projectTree;

		QString		projectDir;
		QString		projectFile;
		QString		projectName;

		QDomDocument	projectData;

		Domain*			fullDomain;	/**< This project's full domain */
		Domain*			currSubDomain;	/**< The subdomain currently being used */
		std::vector<Domain*>	allSubDomains;	/**< List of this project's subdomains */

		/* Flags */
		bool	projectOpen;

		/* Project-wide functionality */
		void	UpdateTreeDisplay();
		void	CreateFullDomain();

		/* Creating a new project */
		bool	CreateProjectFile(QString directory, QString filename);

		/* Opening a project */
		bool	OpenProjectDialog();
		bool	ReadProjectFile(QString filePath);
		void	ReadAllProjectData();
		void	ReadFullDomainData();
		void	ReadSubDomainData();

		void	ReadFullDomainInfo(QDomNodeList nodeList);
		void	ReadSubDomainInfo(QDomNodeList nodeList);

		/* Helper functions */
		bool	ProjectFileExists(QString checkDirectory);
		void	SetFullDomainAttribute(QDomElement dat);
		bool	CheckForValidFile(QString filePath);

		/* Warning dialogs */
		bool	WarnProjectAlreadyOpen();
		void	WarnUnableToCreateDir(QString directory);
		void	WarnProjectAlreadyExists(QString directory);
		void	WarnUnableToCreateFile(QString filename);
		void	WarnFileError(QString message);

};

#endif // PROJECT_H
