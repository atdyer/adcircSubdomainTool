#include "Fort015_new.h"

/*
 * Default empty constructor
 */
Fort015_new::Fort015_new(QObject *parent) :
	QObject(parent),
	domainName(),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(0),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
}


/*
 * Full Domain constructor
 */
Fort015_new::Fort015_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(true),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
	targetFile = projectFile->GetFullDomainFort015();
	if (targetFile.isEmpty())
	{
		targetFile = projectFile->GetFullDomainDirectory() + QDir::separator() + "fort.015";
	} else {
		ReadFile();
	}
}


/*
 * Subdomain constructor
 */
Fort015_new::Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	boundaryNodes(),
	enforceBN(0),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(0),
	subdomainApproach(0),
	targetFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainFort015(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "fort.015";
				projectFile->SetSubDomainFort015(domainName, targetFile);
			} else {
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}


	targetFile = projectFile->GetSubDomainFort015(domainName);
	ReadFile();
}


///*
// * New subdomain constructor
// */
//Fort015_new::Fort015_new(QString domainName, ProjectFile_new *projectFile, QString targetDir, QObject *parent) :
//	QObject(parent),
//	domainName(domainName),
//	boundaryNodes(),
//	enforceBN(0),
//	innerBoundaryNodes(),
//	isFullDomainFile(false),
//	outerBoundaryNodes(),
//	projectFile(projectFile),
//	recordFrequency(0),
//	subdomainApproach(0),
//	targetFile()
//{
//	targetFile = targetDir + QDir::separator() + "fort.015";
//}


void Fort015_new::AddBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		boundaryNodes.insert(*it);
	}
}


void Fort015_new::AddInnerBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		innerBoundaryNodes.insert(*it);
	}
}


void Fort015_new::AddOuterBoundaryNodes(std::vector<unsigned int> newNodes)
{
	for (std::vector<unsigned int>::iterator it = newNodes.begin(); it != newNodes.end(); ++it)
	{
		outerBoundaryNodes.insert(*it);
	}
}


bool Fort015_new::FileExists()
{
	return QFile(targetFile).exists();
}


std::set<unsigned int> Fort015_new::GetBoundaryNodes()
{
//	if (!boundaryNodes.size())
//		ReadFile();
	return boundaryNodes;
}


std::set<unsigned int> Fort015_new::GetInnerBoundaryNodes()
{
//	if (!innerBoundaryNodes.size())
//		ReadFile();
	return innerBoundaryNodes;
}


std::set<unsigned int> Fort015_new::GetOuterBoundaryNodes()
{
//	if (!outerBoundaryNodes.size())
//		ReadFile();
	return outerBoundaryNodes;
}


int Fort015_new::GetRecordFrequency()
{
	return recordFrequency;
}


int Fort015_new::GetSubdomainApproach()
{
	return subdomainApproach;
}


void Fort015_new::SetSubdomainApproach(int approach)
{
	subdomainApproach = approach;
}


void Fort015_new::SetRecordFrequency(int frequency)
{
	recordFrequency = frequency;
}


void Fort015_new::WriteFile()
{
	std::ofstream file (targetFile.toStdString().data());
	if (file.is_open())
	{
		if (isFullDomainFile)
		{
			file << subdomainApproach << "\t!NOUTGS\n";
			file << recordFrequency << "\tNSPOOLGS\n";
			file << "0\t!enforceBN\n";
			file << outerBoundaryNodes.size() << "\t!nobnr\n";
			for (std::set<unsigned int>::iterator it = outerBoundaryNodes.begin(); it != outerBoundaryNodes.end(); ++it)
			{
				file << *it << "\n";
			}
			file << innerBoundaryNodes.size() << "\t!nibnr\n";
			for (std::set<unsigned int>::iterator it = innerBoundaryNodes.begin(); it != innerBoundaryNodes.end(); ++it)
			{
				file << *it << "\n";
			}
			file.close();
			projectFile->SetFullDomainFort015(targetFile);
		} else {
			file << "0\t!NOUTGS\n";
			file << "0\t!NSPOOLGS\n";
			file << subdomainApproach << "\t!enforceBN\n";
			file << "0\n";
			file << "0\n";
			file << "0\n";
			file.close();
			projectFile->SetSubDomainFort015(domainName, targetFile);
		}
	}
}


void Fort015_new::ReadFile()
{
	std::ifstream file (targetFile.toStdString().data());
	if (file.is_open() && isFullDomainFile)
	{
		innerBoundaryNodes.clear();
		outerBoundaryNodes.clear();

		std::string line;
		std::getline(file, line);
		std::stringstream(line) >> subdomainApproach;
		std::getline(file, line);
		std::stringstream(line) >> recordFrequency;
		std::getline(file, line);
		std::stringstream(line) >> enforceBN;

		if (subdomainApproach == 1)
		{
			std::getline(file, line);
			int numBN;
			std::stringstream(line) >> numBN;
			int currNode;
			for (int i=0; i<numBN; ++i)
			{
				file >> currNode;
				boundaryNodes.insert(currNode);
			}
		}
		else if (subdomainApproach == 2)
		{
			std::getline(file, line);
			int numOuterBN;
			std::stringstream(line) >> numOuterBN;
			int currNode;
			for (int i=0; i<numOuterBN; ++i)
			{
				file >> currNode;
				outerBoundaryNodes.insert(currNode);
			}
			std::getline(file, line);
			std::getline(file, line);
			int numInnerBN;
			std::stringstream(line) >> numInnerBN;
			for (int i=0; i<numInnerBN; ++i)
			{
				file >> currNode;
				innerBoundaryNodes.insert(currNode);
			}
		}

		file.close();
	}
	else if (file.is_open() && !isFullDomainFile)
	{
		std::string line;
		std::getline(file, line);
		std::getline(file, line);
		file >> subdomainApproach;
		file.close();
	}
}


//QString Fort015_new::GetFilePath()
//{
//	QString targetFile ("");
//	if (projectFile)
//	{
//		if (isFullDomainFile)
//		{
//			targetFile = projectFile->GetFullDomainFort015();
//		}
//		else if (!domainName.isEmpty())
//		{
//			targetFile = projectFile->GetSubDomainFort015(domainName);
//		}

//		if (targetFile.isEmpty() && (isFullDomainFile || !domainName.isEmpty()))
//		{
//			QDir targetDirectory (isFullDomainFile ? projectFile->GetFullDomainDirectory() : projectFile->GetSubDomainDirectory(domainName));
//			if (targetDirectory.exists())
//			{
//				targetFile = targetDirectory.absolutePath() + QDir::separator() + "fort.015";
//				isFullDomainFile ? projectFile->SetFullDomainFort015(targetFile) : projectFile->SetSubDomainFort015(domainName, targetFile);
//			}
//		}
//	}
//	return targetFile;
//}
