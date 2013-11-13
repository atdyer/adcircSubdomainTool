#include "Py141_new.h"


/*
 * Default constructor
 */
Py141_new::Py141_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0),
	targetFile(),
	newToOldElements(),
	oldToNewElements()
{

}


/*
 * Subdomain constructor
 */
Py141_new::Py141_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile),
	targetFile(),
	newToOldElements(),
	oldToNewElements()
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainPy141(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "py.141";
				projectFile->SetSubDomainPy141(domainName, targetFile);
			} else {
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}
}


void Py141_new::SaveFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		std::ofstream file (projectFile->GetSubDomainPy141(domainName).toStdString().data());
		if (file.is_open())
		{
			file << "new old " << newToOldElements.size() << "\n";
			if (newToOldElements.size() > 0)
			{
				for (std::map<unsigned int, unsigned int>::iterator it = newToOldElements.begin(); it != newToOldElements.end(); ++it)
				{
					file << it->first << " " << it->second << "\n";
				}
			}
			file.close();
		} else {
			std::cout << "Unable to write py.141: " << projectFile->GetSubDomainPy141(domainName).toStdString() << std::endl;
		}
		projectFile->SetSubDomainPy141(domainName, targetFile);
	}
}


void Py141_new::SetNewToOld(std::map<unsigned int, unsigned int> newMap)
{
	newToOldElements = newMap;
	oldToNewElements.clear();
	if (newToOldElements.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = newToOldElements.begin(); it != newToOldElements.end(); ++it)
		{
			oldToNewElements[it->second] = it->first;
		}
	}
}


void Py141_new::SetOldToNew(std::map<unsigned int, unsigned int> newMap)
{
	oldToNewElements = newMap;
	newToOldElements.clear();
	if (oldToNewElements.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = oldToNewElements.begin(); it != oldToNewElements.end(); ++it)
		{
			newToOldElements[it->second] = it->first;
		}
	}
}


unsigned int Py141_new::ConvertNewToOld(unsigned int newNum)
{
	if (!newToOldElements.size())
		ReadFile();
	return newToOldElements[newNum];
}


std::vector<unsigned int> Py141_new::ConvertNewToOld(std::vector<unsigned int> newVector)
{
	if (!newToOldElements.size())
		ReadFile();
	std::vector<unsigned int> oldVals;
	for (std::vector<unsigned int>::iterator it = newVector.begin(); it != newVector.end(); ++it)
	{
		oldVals.push_back(newToOldElements[*it]);
	}
	return oldVals;
}


unsigned int Py141_new::ConvertOldToNew(unsigned int oldNum)
{
	if (!oldToNewElements.size())
		ReadFile();
	return oldToNewElements[oldNum];
}


std::vector<unsigned int> Py141_new::ConvertOldToNew(std::vector<unsigned int> oldVector)
{
	if (!oldToNewElements.size())
		ReadFile();
	std::vector<unsigned int> newVals;
	for (std::vector<unsigned int>::iterator it = oldVector.begin(); it != oldVector.end(); ++it)
	{
		newVals.push_back(oldToNewElements[*it]);
	}
	return newVals;
}


QString	Py141_new::GetFilePath()
{
	if (projectFile && !domainName.isEmpty())
	{
		return projectFile->GetSubDomainPy141(domainName);
	}
	return QString();
}


std::map<unsigned int, unsigned int> Py141_new::GetNewToOld()
{
	if (!newToOldElements.size())
		ReadFile();
	return newToOldElements;
}


std::map<unsigned int, unsigned int> Py141_new::GetOldToNew()
{
	if (!oldToNewElements.size())
		ReadFile();
	return oldToNewElements;
}


void Py141_new::ReadFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		QString targetFile = projectFile->GetSubDomainPy141(domainName);
		std::ifstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			newToOldElements.clear();
			oldToNewElements.clear();
			std::string line;
			std::getline(file, line);
			int currOld, currNew;
			while (std::getline(file, line))
			{
				std::stringstream(line) >> currNew >> currOld;
				newToOldElements[currNew] = currOld;
				oldToNewElements[currOld] = currNew;
			}
			file.close();
		}
	}
}
