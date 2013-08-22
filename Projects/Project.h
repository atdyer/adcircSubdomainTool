#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QObject>
#include <QDir>
#include <QMessageBox>
#include <QDomDocument>

#include "Domains/Domain.h"

class Project : public QObject
{
		Q_OBJECT
	public:
		Project();
		Project(QString projFile);
		~Project();

		void	SetProjectDirectory(QDir newDirectory);
		void	SetFullFort14(QDir newPath);
		void	SetFullFort15(QDir newPath);
		void	SetFullFort63(QDir newPath);

		unsigned int	CreateNewSubdomain(QString newName);

	private:

		QDir	projectDir;

		Domain*			fullDomain;	/**< This project's full domain */
		std::vector<Domain*>	subDomains;	/**< List of this project's subdomains */


		void	ReadProjectFile(QString filePath);
		void	ReadFullDomainInfo(QDomNodeList nodeList);
		void	ReadSubDomainInfo(QDomNodeList nodeList);
};

#endif // PROJECT_H
