#include "Py140_new.h"


/*
 * Default constructor
 */
Py140_new::Py140_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0),
	targetFile(),
	newToOldNodes(),
	oldToNewNodes()
{

}


/*
 * Subdomain constructor
 */
Py140_new::Py140_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile),
	targetFile(),
	newToOldNodes(),
	oldToNewNodes()
{
	if (projectFile && !domainName.isEmpty())
	{
		targetFile = projectFile->GetSubDomainPy140(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "py.140";
				projectFile->SetSubDomainPy140(domainName, targetFile);
			} else {
				// Throw a warning that the subdomain directory doesn't exist
			}
		} else {
			ReadFile();
		}
	}
}


void Py140_new::SaveFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		std::ofstream file (projectFile->GetSubDomainPy140(domainName).toStdString().data());
		if (file.is_open())
		{
			file << "new old " << newToOldNodes.size() << "\n";
			if (newToOldNodes.size() > 0)
			{
				for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
				{
					file << it->first << " " << it->second << "\n";
				}
			}
			file.close();
		} else {
			std::cout << "Unable to write py.140: " << projectFile->GetSubDomainPy140(domainName).toStdString() << std::endl;
		}
		projectFile->SetSubDomainPy140(domainName, targetFile);
	}
}


void Py140_new::SetNewToOld(std::map<unsigned int, unsigned int> newMap)
{
	newToOldNodes = newMap;
	oldToNewNodes.clear();
	if (newToOldNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
		{
			oldToNewNodes[it->second] = it->first;
		}
	}
}


void Py140_new::SetOldToNew(std::map<unsigned int, unsigned int> newMap)
{
	oldToNewNodes = newMap;
	newToOldNodes.clear();
	if (oldToNewNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = oldToNewNodes.begin(); it != oldToNewNodes.end(); ++it)
		{
			newToOldNodes[it->second] = it->first;
		}
	}
}


unsigned int Py140_new::ConvertNewToOld(unsigned int newNum)
{
	if (!newToOldNodes.size())
		ReadFile();
	return newToOldNodes[newNum];
}


std::vector<unsigned int> Py140_new::ConvertNewToOld(std::vector<unsigned int> newVector)
{
	if (!newToOldNodes.size())
		ReadFile();
	std::vector<unsigned int> oldVals;
	for (std::vector<unsigned int>::iterator it = newVector.begin(); it != newVector.end(); ++it)
	{
		oldVals.push_back(newToOldNodes[*it]);
	}
	return oldVals;
}


unsigned int Py140_new::ConvertOldToNew(unsigned int oldNum)
{
	if (!oldToNewNodes.size())
		ReadFile();
	return oldToNewNodes[oldNum];
}


std::vector<unsigned int> Py140_new::ConvertOldToNew(std::vector<unsigned int> oldVector)
{
	if (!oldToNewNodes.size())
		ReadFile();
	std::vector<unsigned int> newVals;
	for (std::vector<unsigned int>::iterator it = oldVector.begin(); it != oldVector.end(); ++it)
	{
		newVals.push_back(oldToNewNodes[*it]);
	}
	return newVals;
}


QString	Py140_new::GetFilePath()
{
	if (projectFile && !domainName.isEmpty())
	{
		return projectFile->GetSubDomainPy140(domainName);
	}
	return QString();
}


std::map<unsigned int, unsigned int> Py140_new::GetNewToOld()
{
	if (!newToOldNodes.size())
		ReadFile();
	return newToOldNodes;
}


std::map<unsigned int, unsigned int> Py140_new::GetOldToNew()
{
	if (!oldToNewNodes.size())
		ReadFile();
	return oldToNewNodes;
}


bool Py140_new::HasOldNode(unsigned int nodeNum)
{
	if (oldToNewNodes.count(nodeNum))
		return true;
	return false;
}


void Py140_new::ReadFile()
{
	if (projectFile && !domainName.isEmpty())
	{
		QString targetFile = projectFile->GetSubDomainPy140(domainName);
		std::ifstream file (targetFile.toStdString().data());
		if (file.is_open())
		{
			newToOldNodes.clear();
			oldToNewNodes.clear();
			std::string line;
			std::getline(file, line);
			int currOld, currNew;
			while (std::getline(file, line))
			{
				std::stringstream(line) >> currNew >> currOld;
				newToOldNodes[currNew] = currOld;
				oldToNewNodes[currOld] = currNew;
			}
			file.close();
		}
	}
}
