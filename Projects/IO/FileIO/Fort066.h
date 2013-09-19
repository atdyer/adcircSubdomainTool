#ifndef FORT066_H
#define FORT066_H

#include <map>
#include <vector>

#include <QString>
#include <QDir>
#include <QFileInfo>

#include "Domains/Domain.h"
#include "Projects/IO/FileIO/Py140.h"
#include "Projects/IO/FileIO/Fort020.h"

class Fort066
{
	public:
		Fort066();
		Fort066(QString newLoc);
		~Fort066();

		void	SetFilePath(QString newLoc);
		void	SetSubdomains(std::vector<Domain*> newDomains);

		void	CarveAllSubdomains();

	private:

		QString	filePath;

		int	numNodesRecorded;
		int	numTSRecorded;
		int	currentTimestep;

		std::vector<Domain*>		subdomains;
		std::map<Domain*, Fort020*>	fortMaps;
		std::map<Domain*, Py140*>	nodeMaps;

		/* Reading fort.066 */
		void	OpenFile();
		void	ReadTimestep();

		/* Writing fort.020 */
		void	CreateFort020File(Domain* currDomain);
		void	GetPy140File(Domain* currDomain);
		void	WriteFort020FileInfoLines(Domain* currDomain);
		void	WriteFort020Timestep(Domain* currDomain);

};

#endif // FORT066_H
