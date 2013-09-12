#ifndef FULLDOMAINRUNNER_H
#define FULLDOMAINRUNNER_H

#include "Domains/Domain.h"

#include "Dialogs/FullDomainRunOptionsDialog.h"

#include "Projects/IO/FileIO/Fort015.h"

#include <QString>
#include <iostream>

class FullDomainRunner
{
	public:
		FullDomainRunner();
		~FullDomainRunner();

		void	SetAdcircExecutable(QString newLoc);
		void	SetFullDomain(Domain *newFull);
		void	SetSubDomains(std::vector<Domain*> newSubs);

		bool	PrepareForFullDomainRun();
		bool	PerformFullDomainRun();

	private:

		Domain*			fullDomain;
		QString			fullDomainPath;
		std::vector<Domain*>	subDomains;
		QString			adcircExecutableLocation;

		int	subdomainApproach;
		int	recordFrequency;
		int	runEnvironment;
		std::vector<unsigned int>	innerBoundaries;
		std::vector<unsigned int>	outerBoundaries;

		void	DisplayFullDomainOptionsDialog();

		bool	CheckForRequiredFiles();
		bool	WriteFort015File();
};

#endif // FULLDOMAINRUNNER_H
