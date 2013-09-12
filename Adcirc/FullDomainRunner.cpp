#include "FullDomainRunner.h"

FullDomainRunner::FullDomainRunner()
{
	fullDomain = 0;
	fullDomainPath = "";
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
	if (newFull)
	{
		fullDomain = newFull;
		fullDomainPath = fullDomain->GetDomainPath();
	}
}


void FullDomainRunner::SetSubDomains(std::vector<Domain *> newSubs)
{
	if (newSubs.size() > 0)
	{
		subDomains = newSubs;
	}
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
		if (!WriteFort015File())
		{
			std::cout << "Did not write fort.015 file" << std::endl;
			return false;
		}
		return CheckForRequiredFiles();
	}
	return false;
}


bool FullDomainRunner::PerformFullDomainRun()
{
	return false;
}


bool FullDomainRunner::CheckForRequiredFiles()
{
	/* Check for fort.14, fort.15, fort.015, ln to adcirc */
	return true;
}


bool FullDomainRunner::WriteFort015File()
{
	Fort015 fort015;
	fort015.SetPath(fullDomainPath);
	fort015.SetSubdomains(subDomains);
	fort015.SetApproach(subdomainApproach);
	fort015.SetRecordFrequency(recordFrequency);
	return fort015.WriteFort015FullDomain();
}
