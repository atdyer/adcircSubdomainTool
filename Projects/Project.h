#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QObject>
#include <QDir>
#include <QMessageBox>
#include <QDomDocument>

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
		Project(QWidget *parent = 0);
		Project(QString projFile, QWidget *parent = 0);
		~Project();

		/* Project-wide functionality */

		/* Creating a new project */
		bool	CreateProject();
		bool	CreateProject(QString directory, QString projectName);

		/* Opening a project */

		/* Editing a project */
		void	SetFullFort14(QString newPath);
		void	SetFullFort15(QString newPath);
		void	SetFullFort63(QString newPath);

		unsigned int	CreateNewSubdomain(QString newName);

	private:

		QWidget*	parentWidget;
		QString		projectDir;
		QString		projectFile;

		Domain*			fullDomain;	/**< This project's full domain */
		std::vector<Domain*>	subDomains;	/**< List of this project's subdomains */

		/* Creating a new project */
		void	CreateProjectFile(QString directory, QString filename);

		bool	ProjectFileExists(QString checkDirectory);
		void	WarnUnableToCreateDir(QString directory);
		void	WarnProjectAlreadyExists(QString directory);

		/* Opening a project */
		void	ReadProjectFile(QString filePath);
		void	ReadFullDomainInfo(QDomNodeList nodeList);
		void	ReadSubDomainInfo(QDomNodeList nodeList);
};

#endif // PROJECT_H
