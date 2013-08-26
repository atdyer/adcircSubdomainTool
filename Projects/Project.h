#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QObject>
#include <QTreeWidget>

#include "Dialogs/CreateProjectDialog.h"

#include "Domains/Domain.h"

#include "Projects/ProjectFile.h"


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
		void	CreateProject();

		/* Opening a project */
		void	OpenProject();

		/* Editing a project */
		bool	ProjectIsOpen();
		void	SetFullFort14(QString newPath);
		void	SetFullFort15(QString newPath);
		void	SetFullFort63(QString newPath);

		unsigned int	CreateNewSubdomain(QString newName);

		/* Active domain fetching functions */
		Domain*		GetFullDomain();
		Domain*		GetActiveSubdomain();

	private:

		QTreeWidget*	projectTree;

		ProjectFile	testProjectFile;

		Domain*			fullDomain;	/**< This project's full domain */
		Domain*			currSubDomain;	/**< The subdomain currently being used */
		std::vector<Domain*>	allSubDomains;	/**< List of this project's subdomains */

		/* Project-wide functionality */
		void	UpdateTreeDisplay();
		void	CreateFullDomain();

		/* Creating a new project */
		bool	CreateProjectFile(QString directory, QString filename);

		/* Opening a project */
		void	PopulateFromProjectFile();

};

#endif // PROJECT_H
