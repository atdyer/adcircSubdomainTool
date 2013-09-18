#include "Py140.h"

Py140::Py140()
{
	filePath = "";
	numFullNodes = 0;
}


Py140::~Py140()
{

}


void Py140::SetFilePath(QString newPath)
{
	if (QFile(newPath).exists())
		filePath = newPath;
}


void Py140::SetOldToNew(std::map<unsigned int, unsigned int> newMap)
{
	oldToNewNodes = newMap;
	FillNewToOld();
}


void Py140::SetNewToOld(std::map<unsigned int, unsigned int> newMap)
{
	newToOldNodes = newMap;
	FillOldToNew();
}


void Py140::SetNumFullNodes(int newCount)
{
	numFullNodes = newCount;
}


void Py140::ReadFile()
{
	std::ifstream file (filePath.toStdString().data());
	if (file.is_open())
	{
		newToOldNodes.clear();
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


void Py140::WriteFile()
{
	std::ofstream file (filePath.toStdString().data());
	if (file.is_open())
	{
		file << "new old " << numFullNodes << std::endl;
		if (newToOldNodes.size() != 0)
		{
			for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
			{
				file << it->first << " " << it->second << std::endl;
			}
		}
		file.close();
	} else {
		std::cout << "Unable to open " << filePath.toStdString().data() << " for writing" << std::endl;
	}
}


std::map<unsigned int, unsigned int> Py140::GetOldToNew()
{
	return oldToNewNodes;
}


std::map<unsigned int, unsigned int> Py140::GetNewToOld()
{
	return newToOldNodes;
}


std::vector<unsigned int> Py140::ConvertNewToOld(std::vector<unsigned int> newList)
{
	std::vector<unsigned int> oldList;
	for (std::vector<unsigned int>::iterator it = newList.begin(); it != newList.end(); ++it)
	{
		oldList.push_back(newToOldNodes[*it]);
	}
	return oldList;
}


std::vector<unsigned int> Py140::ConvertOldToNew(std::vector<unsigned int> oldList)
{
	std::vector<unsigned int> newList;
	for (std::vector<unsigned int>::iterator it = oldList.begin(); it != oldList.end(); ++it)
	{
		newList.push_back(oldToNewNodes[*it]);
	}
	return newList;
}


unsigned int Py140::ConvertNewToOld(unsigned int newNum)
{
	return newToOldNodes[newNum];
}


unsigned int Py140::ConvertOldToNew(unsigned int oldNum)
{
	return oldToNewNodes[oldNum];
}


void Py140::FillNewToOld()
{
	newToOldNodes.clear();
	if (oldToNewNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = oldToNewNodes.begin(); it != oldToNewNodes.end(); ++it)
		{
			newToOldNodes[it->second] = it->first;
		}
	}
}


void Py140::FillOldToNew()
{
	oldToNewNodes.clear();
	if (newToOldNodes.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = newToOldNodes.begin(); it != newToOldNodes.end(); ++it)
		{
			oldToNewNodes[it->second] = it->first;
		}
	}
}
