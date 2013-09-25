#include "Fort067_new.h"

Fort067_new::Fort067_new(QObject *parent) :
	QObject(parent),
	currentData(),
	currTS(0),
	file(),
	numNodes(0),
	numTS(0),
	projectFile(0)
{
}


Fort067_new::Fort067_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	currentData(),
	currTS(0),
	file(),
	numNodes(0),
	numTS(0),
	projectFile(projectFile)
{
	if (projectFile)
	{
		QString targetFile = projectFile->GetFullDomainFort067();
		if (!targetFile.isEmpty())
		{
			SearchForFile();
		}
	}
}


bool Fort067_new::StartReading()
{
	if (projectFile)
	{
		QString targetFile = projectFile->GetFullDomainFort067();
		file.open(targetFile.toStdString().data());
		if (file.is_open())
		{
			int NSPOOLGS;
			std::string line;
			std::getline(file, line);
			std::getline(file, line);
			std::stringstream(line) >> NSPOOLGS >> numNodes >> numTS;
			if (numNodes && numTS)
			{
				return true;
			} else {
				file.close();
				return false;
			}
		}
	}
	return false;
}


bool Fort067_new::HasNextTimestep()
{
	return currTS < numTS;
}


std::map<unsigned int, std::string> Fort067_new::GetNextTimestep()
{
	currentData.clear();
	if (file.is_open())
	{
		std::string tsLine, line;
		unsigned int currNode;
		std::getline(file, tsLine);
		currentData[0] = tsLine;
		for (int i=0; i<numNodes; ++i)
		{
			file >> currNode;
			std::getline(file, line);
			currentData[currNode] = "\t" + line + "\n";
		}
	}
	++currTS;
	return currentData;
}


int Fort067_new::GetNumTimesteps()
{
	return numTS;
}


void Fort067_new::FinishedReading()
{
	if (file.is_open())
		file.close();
	currentData.clear();
	currTS = 0;
	numNodes = 0;
	numTS = 0;
}


void Fort067_new::SearchForFile()
{
	if (projectFile)
	{
		QDir targetDirectory (projectFile->GetFullDomainDirectory());
		if (targetDirectory.exists())
		{
			if (targetDirectory.exists("fort.067"))
			{
				projectFile->SetFullDomainFort067(targetDirectory.absoluteFilePath("fort.067"));
			}
		}
	}
}
