#include "BNList14.h"

BNList14::BNList14(Domain *parent) :
	parentDomain(parent),
	projectFile(0),
	fileLoc(""),
	numInnerNodes(0),
	numOuterNodes(0),
	innerNodes(),
	outerNodes()
{
}


void BNList14::SetInnerBoundaryNodes(std::vector<unsigned int> newNodes)
{
	innerNodes = newNodes;
	numInnerNodes = innerNodes.size();
}



void BNList14::SetOuterBoundaryNodes(std::vector<unsigned int> newNodes)
{
	outerNodes = newNodes;
	numOuterNodes = outerNodes.size();
}


void BNList14::WriteFile()
{
	std::ofstream file (fileLoc.toStdString().data());
	if (file.is_open())
	{
		file << "0\t!cbn\n";
		file << numOuterNodes << "\t!obn\n";
		for (std::vector<unsigned int>::iterator it = outerNodes.begin(); it != outerNodes.end(); ++it)
		{
			file << *it << "\n";
		}
		file << numInnerNodes << "\t!ibn\n";
		for (std::vector<unsigned int>::iterator it = innerNodes.begin(); it != innerNodes.end(); ++it)
		{
			file << *it << "\n";
		}
		file.close();
	}
}


std::vector<unsigned int> BNList14::GetInnerBoundaryNodes()
{
	return innerNodes;
}


std::vector<unsigned int> BNList14::GetOuterBoundaryNodes()
{
	return outerNodes;
}


unsigned int BNList14::GetNumInnerBoundaryNodes()
{
	return numInnerNodes;
}


unsigned int BNList14::GetNumOuterBoundaryNodes()
{
	return numOuterNodes;
}


void BNList14::GetFilePath()
{
	if (parentDomain)
	{
		/* Get the file path from the parent domain's parent project's project file */
	}
}


void BNList14::ReadFile()
{
	std::ifstream file (fileLoc.toStdString().data());
	if (file.is_open())
	{
		innerNodes.clear();
		outerNodes.clear();
		std::string line;
		std::getline(file, line);
		std::getline(file, line);
		std::stringstream(line) >> numOuterNodes;
		unsigned int i=0;
		unsigned int currNodeNum = 0;
		while (i < numOuterNodes)
		{
			file >> currNodeNum;
			outerNodes.push_back(currNodeNum);
			++i;
		}
		std::getline(file, line);
		std::stringstream(line) >> numInnerNodes;
		i = 0;
		while (i < numInnerNodes)
		{
			file >> currNodeNum;
			innerNodes.push_back(currNodeNum);
			++i;
		}
		file.close();
	}
}
