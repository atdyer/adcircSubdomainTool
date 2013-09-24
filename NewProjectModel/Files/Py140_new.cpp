#include "Py140_new.h"

Py140_new::Py140_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0),
	newToOldNodes(),
	oldToNewNodes()
{

}


Py140_new::Py140_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile),
	newToOldNodes(),
	oldToNewNodes()
{
	if (projectFile && !domainName.isEmpty())
	{
		QString targetFile = projectFile->GetSubDomainPy140(domainName);
		if (targetFile.isEmpty())
		{
			QString targetDirectory = projectFile->GetSubDomainDirectory(domainName);
			if (!targetDirectory.isEmpty() && QDir(targetDirectory).exists())
			{
				targetFile = targetDirectory + QDir::separator() + "py.140";
				projectFile->SetSubDomainPy140(domainName, targetFile);
			}
		}
	}
}


std::map<unsigned int, unsigned int> Py140_new::GetNewToOld()
{
	return newToOldNodes;
}


std::map<unsigned int, unsigned int> Py140_new::GetOldToNew()
{
	return oldToNewNodes;
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
	return newToOldNodes[newNum];
}


std::vector<unsigned int> Py140_new::ConvertNewToOld(std::vector<unsigned int> newVector)
{
	std::vector<unsigned int> oldVals;
	for (std::vector<unsigned int>::iterator it = newVector.begin(); it != newVector.end(); ++it)
	{
		oldVals.push_back(newToOldNodes[*it]);
	}
	return oldVals;
}


unsigned int Py140_new::ConvertOldToNew(unsigned int oldNum)
{
	return oldToNewNodes[oldNum];
}


std::vector<unsigned int> Py140_new::ConvertOldToNew(std::vector<unsigned int> oldVector)
{
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
