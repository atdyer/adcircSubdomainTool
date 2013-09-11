#ifndef FULLDOMAINRUNNER_H
#define FULLDOMAINRUNNER_H

#include "Domains/Domain.h"

#include "Dialogs/FullDomainRunOptionsDialog.h"

#include <QString>
#include <iostream>

class FullDomainRunner
{
	public:
		FullDomainRunner();
		~FullDomainRunner();

		void	SetAdcircExecutable(QString newLoc);
		void	SetFullDomain(Domain *newFull);
		void	SetSubDomains(std::vector<Domain*> subDomains);

		bool	PrepareForFullDomainRun();
		bool	PerformFullDomainRun();

	private:

		Domain*			fullDomain;
		std::vector<Domain*>	subDomains;
		QString			adcircExecutableLocation;

		int	subdomainApproach;
		int	recordFrequency;
		int	runEnvironment;
		std::vector<unsigned int>	innerBoundaries;
		std::vector<unsigned int>	outerBoundaries;

		void	DisplayFullDomainOptionsDialog();

		bool	ExtractAllInnerBoundaryNodes();
		bool	ExtractAllOuterBoundaryNodes();
		bool	CheckForRequiredFiles();
		bool	WriteFort015File();
};

#endif // FULLDOMAINRUNNER_H
