#include "BNList14_new.h"

BNList14_new::BNList14_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{
}


BNList14_new::BNList14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{
}


void BNList14_new::CreateFile(QString filePath)
{
	if (projectFile && !domainName.isNull())
	{
		projectFile->SetSubDomainBNList(domainName, filePath);
		SaveFile();
	}
}


void BNList14_new::OpenFile()
{
	if (projectFile && !domainName.isNull())
	{
		QString fileLocation = projectFile->GetSubDomainBNList(domainName);
		if (!fileLocation.isEmpty())
		{
			ReadFile(fileLocation);
		}
	}
}


void BNList14_new::SetInnerBoundaryNodes(std::vector<unsigned int> newNodes)
{
	innerNodes = newNodes;
}


void BNList14_new::SetOuterBoundaryNodes(std::vector<unsigned int> newNodes)
{
	outerNodes = newNodes;
}


void BNList14_new::SaveFile()
{
	if (projectFile && !domainName.isNull())
	{
		std::ofstream file (projectFile->GetSubDomainBNList(domainName).toStdString().data());
		if (file.is_open())
		{
			file << "0\t!cbn\n";
			file << outerNodes.size() << "\t!obn\n";
			for (std::vector<unsigned int>::iterator it = outerNodes.begin(); it != outerNodes.end(); ++it)
			{
				file << *it << "\n";
			}
			file << innerNodes.size() << "\t!ibn\n";
			for (std::vector<unsigned int>::iterator it = innerNodes.begin(); it != innerNodes.end(); ++it)
			{
				file << *it << "\n";
			}
			file.close();
		}
	}
}


QString BNList14_new::GetFilePath()
{
	if (projectFile && !domainName.isNull())
	{
		return projectFile->GetSubDomainBNList(domainName);
	}
	return QString();
}


std::vector<unsigned int> BNList14_new::GetInnerBoundaryNodes()
{
	OpenFile();
	return innerNodes;
}


std::vector<unsigned int> BNList14_new::GetOuterBoundaryNodes()
{
	OpenFile();
	return outerNodes;
}


unsigned int BNList14_new::GetNumInnerBoundaryNodes()
{
	return innerNodes.size();
}


unsigned int BNList14_new::GetNumOuterBoundaryNodes()
{
	return outerNodes.size();
}


void BNList14_new::ReadFile(QString fileLocation)
{
	std::ifstream file (fileLocation.toStdString().data());
	if (file.is_open())
	{
		int numOuterNodes = 0;
		int numInnerNodes = 0;
		int i=0;
		innerNodes.clear();
		outerNodes.clear();
		std::string line;
		std::getline(file, line);
		std::getline(file, line);
		std::stringstream(line) >> numOuterNodes;
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
