#include "Fort015_new.h"

Fort015_new::Fort015_new(QObject *parent) :
	QObject(parent),
	domainName(),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(0),
	recordFrequency(0),
	subdomainApproach(0)
{
}


Fort015_new::Fort015_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	innerBoundaryNodes(),
	isFullDomainFile(true),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(1),
	subdomainApproach(0)
{
}


Fort015_new::Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	innerBoundaryNodes(),
	isFullDomainFile(false),
	outerBoundaryNodes(),
	projectFile(projectFile),
	recordFrequency(0),
	subdomainApproach(2)
{
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


void Fort015_new::SetApproach(int approach)
{
	subdomainApproach = approach;
}


void Fort015_new::SetRecordFrequency(int frequency)
{
	recordFrequency = frequency;
}


void Fort015_new::WriteFile()
{
	std::ofstream file (GetFilePath().toStdString().data());
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
		} else {
			file << "0\t!NOUTGS\n";
			file << "0\t!NSPOOLGS\n";
			file << subdomainApproach << "\t!enforceBN\n";
			file << "0\n";
			file << "0\n";
			file << "0\n";
			file.close();
		}
	}
}


QString Fort015_new::GetFilePath()
{
	QString targetFile ("");
	if (projectFile)
	{
		if (isFullDomainFile)
		{
			targetFile = projectFile->GetFullDomainFort015();
		}
		else if (!domainName.isEmpty())
		{
			targetFile = projectFile->GetSubDomainFort015(domainName);
		}

		if (targetFile.isEmpty() && (isFullDomainFile || !domainName.isEmpty()))
		{
			QDir targetDirectory (isFullDomainFile ? projectFile->GetFullDomainDirectory() : projectFile->GetSubDomainDirectory(domainName));
			if (targetDirectory.exists())
			{
				targetFile = targetDirectory.absolutePath() + QDir::separator() + "fort.015";
				isFullDomainFile ? projectFile->SetFullDomainFort015(targetFile) : projectFile->SetSubDomainFort015(domainName, targetFile);
			}
		}
	}
	return targetFile;
}
