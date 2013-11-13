#ifndef BNLIST14_new_H
#define BNLIST14_new_H

#include <QObject>
#include <QDir>
#include <QFile>

#include <vector>
#include <fstream>
#include <sstream>

#include "NewProjectModel/Files/ProjectFile_new.h"

class BNList14_new : public QObject
{
		Q_OBJECT
	public:
		explicit BNList14_new(QObject *parent=0);
		BNList14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

		void	SetInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetSubdomainVersion(int v);
		void	SaveFile();

		QString				GetFilePath();
		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		unsigned int			GetNumInnerBoundaryNodes();
		unsigned int			GetNumOuterBoundaryNodes();
		int				GetSubdomainVersion();

	private:

		QString				domainName;
		std::vector<unsigned int>	innerNodes;
		std::vector<unsigned int>	outerNodes;
		ProjectFile_new*		projectFile;
		QString				targetFile;
		int				version;

		void	ReadFile();
};

#endif // BNLIST14_new_H
