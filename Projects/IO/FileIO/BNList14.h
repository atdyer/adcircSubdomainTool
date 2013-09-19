#ifndef BNLIST14_H
#define BNLIST14_H

#include <vector>
#include <fstream>
#include <sstream>

#include <QString>

class BNList14
{
	public:
		BNList14();
		BNList14(QString newLoc);

		void	SetFilePath(QString newLoc);
		void	SetInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	WriteFile();

		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		unsigned int			GetNumInnerBoundaryNodes();
		unsigned int			GetNumOuterBoundaryNodes();

	private:

		QString	fileLoc;

		unsigned int	numInnerNodes;
		unsigned int	numOuterNodes;
		std::vector<unsigned int>	innerNodes;
		std::vector<unsigned int>	outerNodes;

		void	ReadFile();
};

#endif // BNLIST14_H
