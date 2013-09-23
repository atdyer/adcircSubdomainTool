#ifndef BNLIST14_new_H
#define BNLIST14_new_H

#include <QObject>

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

		void	CreateFile(QString filePath);
		void	OpenFile();
		void	SetInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SaveFile();

		QString				GetFilePath();
		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		unsigned int			GetNumInnerBoundaryNodes();
		unsigned int			GetNumOuterBoundaryNodes();

	private:

		QString				domainName;
		std::vector<unsigned int>	innerNodes;
		std::vector<unsigned int>	outerNodes;
		ProjectFile_new*		projectFile;

		void	ReadFile(QString fileLocation);
};

#endif // BNLIST14_new_H
