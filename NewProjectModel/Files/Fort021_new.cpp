#include "Fort021_new.h"

Fort021_new::Fort021_new(QObject *parent) :
	QObject(parent),
	domainName(),
	file(),
	projectFile(0)
{
}


Fort021_new::Fort021_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	file(),
	projectFile(projectFile)
{
}


bool Fort021_new::StartWriting()
{
	QString targetFile = GetFilePath();
	if (!targetFile.isEmpty())
	{
		file.open(targetFile.toStdString().data());
		if (file.is_open())
		{
			file << "Boundary conditions for subdomain\n";
			return true;
		}
	}
	return false;
}


void Fort021_new::WriteHeader(int numTS, std::vector<unsigned int> nodeList)
{
	if (file.is_open())
	{
		file << "1\t" << nodeList.size() << "\t" << numTS << "\n";
		for (std::vector<unsigned int>::iterator it = nodeList.begin(); it != nodeList.end(); ++it)
		{
			file << *it << "\n";
		}
	}
}


void Fort021_new::WriteTimestep(std::vector<std::string> tsData)
{
	if (file.is_open())
	{
		for (std::vector<std::string>::iterator it = tsData.begin(); it != tsData.end(); ++it)
		{
			file << *it;
		}
	}
}


void Fort021_new::FinishedWriting()
{
	if (file.is_open())
		file.close();
}


QString Fort021_new::GetFilePath()
{
	if (projectFile && !domainName.isEmpty())
	{
		QString targetFile = projectFile->GetSubDomainFort021(domainName);
		if (targetFile.isEmpty())
		{
			QDir targetDirectory (projectFile->GetSubDomainDirectory(domainName));
			if (targetDirectory.exists())
			{
				targetFile = targetDirectory.absolutePath() + QDir::separator() + "fort.021";
				projectFile->SetSubDomainFort021(domainName, targetFile);
			}
		}
		return targetFile;
	}
	return QString("");
}
