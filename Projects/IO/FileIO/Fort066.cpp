#include "Fort066.h"

Fort066::Fort066()
{
	filePath = "";

	numNodesRecorded = 0;
	numTSRecorded = 0;
	currentTimestep = 0;
}


Fort066::~Fort066()
{

}


void Fort066::SetFilePath(QString newLoc)
{

}


void Fort066::SetSubdomains(std::vector<Domain *> newDomains)
{

}


void Fort066::CarveAllSubdomains()
{
	/* Open the fort.066 file to get the number of TS recorded */
	OpenFile();

	/* Loop through each subdomain:
	 *	- Create the fort.020 file
	 *	- Retrieve the py.140 and bnlist.14 files from the subdomain
	 *	- Write the first chunk of info lines for the subdomain
	 */
	Domain *currDomain = 0;
	for (std::vector<Domain*>::iterator it = subdomains.begin(); it != subdomains.end(); ++it)
	{
		currDomain = *it;
		if (currDomain)
		{
			CreateFort020File(currDomain);
			GetBNList(currDomain);
			GetPy140File(currDomain);
			WriteFort020FileInfoLines(currDomain);
		}
	}

	/* Loop through each timestep of the full domain run:
	 *	- Read the data from the timestep
	 *	- Parse the data and put into a map that associates values with nodes
	 *	Loop through each subdomain:
	 *		- Get all of the data needed from the appropriate nodes
	 *		- Assemble the text of an entire timestep for the subdomain
	 *		- Write the data to the subdomain fort.020 file
	 */
	currentTimestep = 1;
	while (currentTimestep <= numTSRecorded)
	{
		ReadTimestep();
		for (std::vector<Domain*>::iterator it = subdomains.begin(); it != subdomains.end(); ++it)
		{
			currDomain = *it;
			if (currDomain)
			{
				WriteFort020Timestep(currDomain);
			}
		}

		++currentTimestep;
	}

	/* Close all of the newly written fort.020 files */
}


void Fort066::OpenFile()
{
	readFile.open(filePath.toStdString().data());
	if (readFile.is_open())
	{
		std::string firstLine;
		std::getline(readFile, firstLine);
		int trash;
		std::stringstream(firstLine) >> trash >> numNodesRecorded >> numTSRecorded;
	}
}


void Fort066::ReadTimestep()
{
	if (readFile.is_open())
	{
		int currNode;
		std::string line1, line2;
		std::getline(readFile, tsLine);
		for (int i=0; i<numNodesRecorded; ++i)
		{
			readFile >> currNode;
			std::getline(readFile, line1);
			std::getline(readFile, line2);
			currentTimestepData[currNode] = "\t" + line1 + "\n" + line2 + "\n";
		}
	}
}


void Fort066::CloseFile()
{
	if (readFile.is_open())
	{
		readFile.close();
	}
}


void Fort066::CreateFort020File(Domain *currDomain)
{
	if (currDomain && fortMaps.count(currDomain) == 0)
	{
		QString filePath = currDomain->GetDomainPath() + QDir::separator() + "fort.020";
		if (QFile(filePath).exists())
		{
			std::cout << "WARNING: Overwriting fort.020 file at: " << filePath.toStdString().data() << std::endl;
		}
		fortMaps[currDomain] = new Fort020(filePath);
	} else {
		std::cout << "WARNING: No domain defined or domain already associated with fort.020 file" << std::endl;
	}
}


void Fort066::GetBNList(Domain *currDomain)
{
	if (currDomain && bnLists.count(currDomain) == 0)
	{
//		bnLists[currDomain] = new BNList14(currDomain->GetBNListLocation());
	} else {
		std::cout << "WARNING: No domain defined or domain already associated with bnlist.14 file" << std::endl;
	}
}


void Fort066::GetPy140File(Domain *currDomain)
{
	if (currDomain && nodeMaps.count(currDomain) == 0)
	{
		nodeMaps[currDomain] = new Py140(currDomain->GetPy140Location());
	} else {
		std::cout << "WARNING: No domain defined or domain already associated with py.140 file" << std::endl;
	}
}


void Fort066::WriteFort020FileInfoLines(Domain *currDomain)
{
	if (currDomain && bnLists.count(currDomain) && fortMaps.count(currDomain))
	{
		BNList14 *currList = bnLists[currDomain];
		Fort020 *currFort = fortMaps[currDomain];
		std::vector<unsigned int> outerBoundaries = currList->GetOuterBoundaryNodes();

		QString infoLines = "Boundary conditions for subdomain\n";
		infoLines.append("1\t" + QString::number(outerBoundaries.size()) + "\t" + QString::number(numTSRecorded) + "\n");

		for (std::vector<unsigned int>::iterator it = outerBoundaries.begin(); it != outerBoundaries.end(); ++it)
		{
			infoLines.append(QString::number(*it) + "\n");
		}

		currFort->WriteInfoLines(infoLines);
	}
	if (currDomain && nodeMaps.count(currDomain))
	{
		Py140 *currPy140 = nodeMaps[currDomain];

	}

}


void Fort066::WriteFort020Timestep(Domain *currDomain)
{
	if (currDomain && nodeMaps.count(currDomain) && fortMaps.count(currDomain));
	{
		Py140 *currNodeMap = nodeMaps[currDomain];
		Fort020 *currFort = fortMaps[currDomain];
		QString currData (tsLine.data());
		currData.append("\n");
		for (std::map<int, std::string>::iterator it = currentTimestepData.begin(); it != currentTimestepData.end(); ++it)
		{
			currData.append(QString::number(currNodeMap->ConvertOldToNew(it->first)) + it->second.data());
		}

		currFort->WriteTimestep(currData);
	}
}
