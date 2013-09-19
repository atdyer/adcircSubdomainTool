#include "Fort066.h"

Fort066::Fort066()
{

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
	Domain *currDomain = 0;
	for (std::vector<Domain*>::iterator it = subdomains.begin(); it != subdomains.end(); ++it)
	{
		currDomain = *it;
		if (currDomain)
		{
			CreateFort020File(currDomain);
			GetPy140File(currDomain);
			WriteFort020FileInfoLines(currDomain);
		}
	}

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
}


void Fort066::OpenFile()
{

}


void Fort066::ReadTimestep()
{

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
	if (nodeMaps.count(currDomain))
	{
		Py140 *currPy140 = nodeMaps[currDomain];
		QString infoLines = "Boundary conditions for subdomain\n";
	}

}


void Fort066::WriteFort020Timestep(Domain *currDomain)
{

}
