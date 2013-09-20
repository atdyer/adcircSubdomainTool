#ifndef BNLIST14_H
#define BNLIST14_H

#include <vector>
#include <fstream>
#include <sstream>

#include <QObject>
#include <QString>

#include "Domains/Domain.h"
#include "Projects/Project.h"
#include "Projects/ProjectFile.h"

class BNList14 : QObject
{
		Q_OBJECT
	public:
		BNList14(Domain *parent=0);

		void	SetFilePath(QString newLoc);
		void	SetInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	WriteFile();

		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		unsigned int			GetNumInnerBoundaryNodes();
		unsigned int			GetNumOuterBoundaryNodes();

	private:

		Domain*		parentDomain;
		ProjectFile*	projectFile;

		QString	fileLoc;

		unsigned int	numInnerNodes;
		unsigned int	numOuterNodes;
		std::vector<unsigned int>	innerNodes;
		std::vector<unsigned int>	outerNodes;

		void	GetFilePath();
		void	ReadFile();
};

#endif // BNLIST14_H
