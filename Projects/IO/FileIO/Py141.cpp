#include "Py141.h"

Py141::Py141()
{
	filePath = "";
	numFullElements = 0;
}


Py141::~Py141()
{

}


void Py141::SetFilePath(QString newPath)
{
	if (QFile(newPath).exists())
		filePath = newPath;
}


void Py141::SetOldToNew(std::map<unsigned int, unsigned int> newMap)
{
	oldToNewElements = newMap;
	FillNewToOld();
}


void Py141::SetNewToOld(std::map<unsigned int, unsigned int> newMap)
{
	newToOldElements = newMap;
	FillOldToNew();
}


void Py141::SetNumFullElements(int newCount)
{
	numFullElements = newCount;
}


void Py141::ReadFile()
{
	std::ifstream file (filePath.toStdString().data());
	if (file.is_open())
	{
		newToOldElements.clear();
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


void Py141::WriteFile()
{
	std::ofstream file (filePath.toStdString().data());
	if (file.is_open())
	{
		file << "new old " << numFullElements << std::endl;
		if (newToOldElements.size() != 0)
		{
			for (std::map<unsigned int, unsigned int>::iterator it = newToOldElements.begin(); it != newToOldElements.end(); ++it)
			{
				file << it->first << " " << it->second << std::endl;
			}
		}
		file.close();
	} else {
		std::cout << "Unable to open " << filePath.toStdString().data() << " for writing" << std::endl;
	}
}


std::map<unsigned int, unsigned int> Py141::GetOldToNew()
{
	return oldToNewElements;
}


std::map<unsigned int, unsigned int> Py141::GetNewToOld()
{
	return newToOldElements;
}


std::vector<unsigned int> Py141::ConvertNewToOld(std::vector<unsigned int> newList)
{
	std::vector<unsigned int> oldList;
	for (std::vector<unsigned int>::iterator it = newList.begin(); it != newList.end(); ++it)
	{
		oldList.push_back(newToOldElements[*it]);
	}
	return oldList;
}


std::vector<unsigned int> Py141::ConvertOldToNew(std::vector<unsigned int> oldList)
{
	std::vector<unsigned int> newList;
	for (std::vector<unsigned int>::iterator it = oldList.begin(); it != oldList.end(); ++it)
	{
		newList.push_back(oldToNewElements[*it]);
	}
	return newList;
}


unsigned int Py141::ConvertNewToOld(unsigned int newNum)
{
	return newToOldElements[newNum];
}


unsigned int Py141::ConvertOldToNew(unsigned int oldNum)
{
	return oldToNewElements[oldNum];
}


void Py141::FillNewToOld()
{
	newToOldElements.clear();
	if (oldToNewElements.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = oldToNewElements.begin(); it != oldToNewElements.end(); ++it)
		{
			newToOldElements[it->second] = it->first;
		}
	}
}


void Py141::FillOldToNew()
{
	oldToNewElements.clear();
	if (newToOldElements.size() > 0)
	{
		for (std::map<unsigned int, unsigned int>::iterator it = newToOldElements.begin(); it != newToOldElements.end(); ++it)
		{
			oldToNewElements[it->second] = it->first;
		}
	}
}
