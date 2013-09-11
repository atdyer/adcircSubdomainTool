#include "FullDomainRunner.h"

FullDomainRunner::FullDomainRunner()
{
	fullDomain = 0;
	adcircExecutableLocation = "";
	subdomainApproach = -1;
	recordFrequency = -1;
	runEnvironment = -1;
}


FullDomainRunner::~FullDomainRunner()
{

}


void FullDomainRunner::SetAdcircExecutable(QString newLoc)
{
	adcircExecutableLocation = newLoc;
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
	dlg.SetAdcircExecutable(adcircExecutableLocation);
	if (dlg.exec())
	{
		adcircExecutableLocation = dlg.GetAdcircExecutableLocation();
		subdomainApproach = dlg.GetSubdomainApproach();
		recordFrequency = dlg.GetRecordFrequency();
		runEnvironment = dlg.GetRunEnvironment();
		std::cout << subdomainApproach << recordFrequency << runEnvironment << std::endl;
		std::cout << adcircExecutableLocation.toStdString().data() << std::endl;
		return true;
	}
	return false;
}


bool FullDomainRunner::PerformFullDomainRun()
{
	return false;
}


bool FullDomainRunner::ExtractAllInnerBoundaryNodes()
{
	return false;
}


bool FullDomainRunner::ExtractAllOuterBoundaryNodes()
{
	return false;
}


bool FullDomainRunner::CheckForRequiredFiles()
{
	return false;
}


bool FullDomainRunner::WriteFort015File()
{
	return false;
}
