#ifndef PROJECT_H
#define PROJECT_H

#include <vector>

#include <QDir>

#include "Domains/Domain.h"

class Project
{
	public:
		Project();
		Project(QDir projectDir);
		~Project();

		void	SetProjectDirectory(QDir newDirectory);
		void	SetFullFort14(QDir newPath);
		void	SetFullFort15(QDir newPath);
		void	SetFullFort63(QDir newPath);

		int	CreateNewSubdomain(QString newName);

	private:

		QDir	projectDir;

		Domain*			fullDomain;	/**< This project's full domain */
		std::vector<Domain*>	subDomains;	/**< List of this project's subdomains */
};

#endif // PROJECT_H
