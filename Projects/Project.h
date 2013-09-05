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

		/* User Interface Connections */
		void	SetProjectTree(QTreeWidget *newTree);
		void	SetProgressBar(QProgressBar *newBar);

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

		/* Active domain fetching function */
		Domain*		GetActiveDomain();

	private:

		QTreeWidget*	projectTree;
		QProgressBar*	progressBar;

		ProjectFile	testProjectFile;

		/* The visible Domain */
		Domain*				currentDomain;	/**< The Domain the is currently visible to the user */

		/* The actual Domains */
		Domain*				fullDomain;
		std::map<QString, Domain*>	subDomains;	/**< Map of subdomain names to the actual domain */

		/* Project-wide functionality */
		void	ConnectProjectTree();
		void	UpdateTreeDisplay();
		void	CreateFullDomain();

		/* Creating a new project */
		bool	CreateProjectFile(QString directory, QString filename);

		/* Opening a project */
		void	PopulateFromProjectFile();

		/* Navigating within the project */
		void	SetCurrentDomain(Domain *domain);
		Domain *DetermineSelectedDomain(QTreeWidgetItem *item);

	private slots:

		void	on_ProjectTreeItemChanged(QTreeWidgetItem *item, QTreeWidgetItem*);

	public slots:

		void	setDomainSolidOutline(unsigned int domainID, QColor color);
		void	setDomainSolidFill(unsigned int domainID, QColor color);

	signals:

		void	newDomainSelected();

};

#endif // PROJECT_H
