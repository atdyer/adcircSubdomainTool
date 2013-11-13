#ifndef SUBDOMAINCREATOR_NEW_H
#define SUBDOMAINCREATOR_NEW_H

#include <QString>
#include <QObject>

#include "NewProjectModel/Domains/FullDomain.h"
#include "NewProjectModel/Domains/SubDomain.h"
#include "NewProjectModel/Files/ProjectFile_new.h"
#include "NewProjectModel/Files/BNList14_new.h"
#include "NewProjectModel/Files/Py140_new.h"
#include "NewProjectModel/Files/Py141_new.h"
#include "SubdomainTools/BoundarySearch.h"

class SubdomainCreator_new
{
	public:
		SubdomainCreator_new();
		~SubdomainCreator_new();

		bool	CreateSubdomain(QString newName,
					ProjectFile_new *projFile,
					QString targetDir,
					FullDomain *fDomain,
					int version,
					int recordFrequency);

	private:

		BNList14_new*				bnList;
		Fort015_new*				fort015Full;
		Fort015_new*				fort015Sub;
		FullDomain*				fullDomain;
		std::vector<unsigned int>		innerBoundaryNodes;
		std::vector<unsigned int>		outerBoundaryNodes;
		Py140_new*				py140;
		Py141_new*				py141;
		ProjectFile_new*			projectFile;
		std::vector<Element*>			selectedElements;
		std::vector<Node*>			selectedNodes;
		QString					subdomainName;

		bool	CheckForExistingSubdomainFiles(QString targetDir);
		void	FindBoundaries(int version);
		void	FindUniqueNodes();
		void	MapOldToNewElements();
		void	MapOldToNewNodes();
		bool	WriteFort14();

		bool	WarnSubdomainFilesExist(QString targetDir);
};

#endif // SUBDOMAINCREATOR_NEW_H
