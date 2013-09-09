#ifndef SUBDOMAINCREATOR_H
#define SUBDOMAINCREATOR_H

#include <QString>
#include <QDir>

#include "Domains/Domain.h"
#include "SubdomainTools/BoundaryFinder.h"

class SubdomainCreator
{
	public:
		SubdomainCreator();

		void	CreateSubdomain();

		void	SetDomain(Domain *newDomain);
		void	SetTargetPath(QString newTarget);


	private:

		BoundaryFinder	boundaryFinder;

		ElementState*			currentSelectedState;
		std::vector<unsigned int>	boundaryNodes;

		QString	targetPath;

		void	WriteFort14File();

};

#endif // SUBDOMAINCREATOR_H
