#ifndef FULLDOMAINRUNNER_H
#define FULLDOMAINRUNNER_H

#include "Domains/Domain.h"

#include <QString>

class FullDomainRunner
{
	public:
		FullDomainRunner();
		~FullDomainRunner();

		void	SetFullDomain(Domain *newFull);
		void	SetSubDomains(std::vector<Domain*> subDomains);

		bool	PrepareForFullDomainRun();
		bool	PerformFullDomainRun();

	private:

		Domain*			fullDomain;
		std::vector<Domain*>	subDomains;
		QString			adcircExecutableLocation;

		std::vector<unsigned int>	innerBoundaries;
		std::vector<unsigned int>	outerBoundaries;

		void	DisplayFullDomainOptionsDialog();

		void	ExtractAllInnerBoundaryNodes();
		void	ExtractAllOuterBoundaryNodes();
		void	WriteFort015File();
};

#endif // FULLDOMAINRUNNER_H
