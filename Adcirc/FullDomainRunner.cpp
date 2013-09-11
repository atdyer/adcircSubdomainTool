#include "FullDomainRunner.h"

FullDomainRunner::FullDomainRunner()
{

}


FullDomainRunner::~FullDomainRunner()
{

}


void FullDomainRunner::SetFullDomain(Domain *newFull)
{

}


void FullDomainRunner::SetSubDomains(std::vector<Domain *> subDomains)
{

}


bool FullDomainRunner::PrepareForFullDomainRun()
{
	FullDomainRunOptionsDialog dlg;
	if (dlg.exec())
	{
		subdomainApproach = dlg.GetSubdomainApproach();
		recordFrequency = dlg.GetRecordFrequency();
		runEnvironment = dlg.GetRunEnvironment();
	}
}


bool FullDomainRunner::PerformFullDomainRun()
{

}


void FullDomainRunner::ExtractAllInnerBoundaryNodes()
{

}


void FullDomainRunner::ExtractAllOuterBoundaryNodes()
{

}


void FullDomainRunner::WriteFort015File()
{

}
