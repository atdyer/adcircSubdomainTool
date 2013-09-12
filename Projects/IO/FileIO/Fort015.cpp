#include "Fort015.h"

Fort015::Fort015()
{
	targetPath = "";
	subdomainApproach = -1;
	recordFrequency = -1;
}

Fort015::~Fort015()
{

}


void Fort015::SetPath(QString newPath)
{
	targetPath = newPath;
}


void Fort015::SetSubdomains(std::vector<Domain *> newList)
{
	subDomains = newList;
}


void Fort015::SetApproach(int approach)
{
	subdomainApproach = approach;
}


void Fort015::SetRecordFrequency(int frequency)
{
	recordFrequency = frequency;
}


bool Fort015::WriteFort015FullDomain()
{
	ExtractAllOuterBoundaryNodes();
	if (subdomainApproach == 2)
	{
		ExtractAllInnerBoundaryNodes();
	}
	if (!targetPath.isEmpty() &&
	    subdomainApproach > 0 &&
	    recordFrequency > 0)
	{
		QString fort015Path = targetPath;
		fort015Path.append(QDir::separator()).append("fort.015");
		std::ofstream fort015;
		fort015.open(fort015Path.toStdString().data());
		if (fort015.is_open())
		{
			fort015 << subdomainApproach << "\t!NOUTGS" << std::endl;
			fort015 << recordFrequency << "\t!NSPOOLGS" << std::endl;
			fort015 << "0\t!enforceBN" << std::endl;
			fort015 << outerBoundaries.size() << "\t!nobnr" << std::endl;
			for (std::vector<unsigned int>::iterator it = outerBoundaries.begin(); it != outerBoundaries.end(); ++it)
			{
				fort015 << *it << std::endl;
			}
			fort015 << innerBoundaries.size() << "\t!nibnr" << std::endl;
			for (std::vector<unsigned int>::iterator it = innerBoundaries.begin(); it != innerBoundaries.end(); ++it)
			{
				fort015 << *it << std::endl;
			}
			fort015.close();
			return true;
		}
	}
	return false;
}


bool Fort015::WriteFort015Subdomain()
{
	if (!targetPath.isEmpty() && subdomainApproach > 0)
	{
		QString fort015Path = targetPath;
		fort015Path.append(QDir::separator()).append("fort.015");
		std::ofstream fort015;
		fort015.open(fort015Path.toStdString().data());
		if (fort015.is_open())
		{
			fort015 << "0\t!NOUTGS" << std::endl;
			fort015 << "0\t!NSPOOLGS" << std::endl;
			fort015 << subdomainApproach << "\t!enforceBN" << std::endl;
			fort015 << "0" << std::endl;
			fort015 << "0" << std::endl;
			fort015 << "0" << std::endl;
			fort015.close();
			return true;
		}
	}
	return false;
}


bool Fort015::ExtractAllInnerBoundaryNodes()
{
	std::cout << "Extracting inner boundary nodes from " << subDomains.size() << " subdomains" << std::endl;
	if (subDomains.size() > 0)
	{
		Domain *currSubdomain = 0;
		for (std::vector<Domain*>::iterator it = subDomains.begin(); it != subDomains.end(); ++it)
		{
			currSubdomain = *it;
			if (currSubdomain)
			{
				std::vector<unsigned int> newBoundaries = boundaryFinder.FindInnerBoundaries(currSubdomain->GetCurrentSelectedElements());
				std::cout << "Found " << newBoundaries.size() << " new inner boundary nodes" << std::endl;
				innerBoundaries.insert(innerBoundaries.end(), newBoundaries.begin(), newBoundaries.end());
			}
		}
		if (innerBoundaries.size() > 0)
		{
			/* Sort the list */
			std::sort(innerBoundaries.begin(), innerBoundaries.end());

			/* Remove duplicates */
			std::vector<unsigned int>::iterator it;
			it = std::unique(innerBoundaries.begin(), innerBoundaries.end());
			innerBoundaries.resize(std::distance(innerBoundaries.begin(), it));
			return true;
		} else {
			return false;
		}
	}
	return false;
}


bool Fort015::ExtractAllOuterBoundaryNodes()
{
	std::cout << "Extracting outer boundary nodes from " << subDomains.size() << " subdomains" << std::endl;
	if (subDomains.size() > 0)
	{
		Domain *currSubdomain = 0;
		for (std::vector<Domain*>::iterator it = subDomains.begin(); it != subDomains.end(); ++it)
		{
			currSubdomain = *it;
			if (currSubdomain)
			{
				std::vector<unsigned int> newBoundaries = boundaryFinder.FindBoundaries(currSubdomain->GetCurrentSelectedElements());
				std::cout << "Found " << newBoundaries.size() << " new outer boundary nodes" << std::endl;
				outerBoundaries.insert(outerBoundaries.end(), newBoundaries.begin(), newBoundaries.end());
			}
		}
		if (outerBoundaries.size() > 0)
		{
			/* Sort the list */
			std::sort(outerBoundaries.begin(), outerBoundaries.end());

			/* Remove duplicates */
			std::vector<unsigned int>::iterator it;
			it = std::unique(outerBoundaries.begin(), outerBoundaries.end());
			outerBoundaries.resize(std::distance(outerBoundaries.begin(), it));
			return true;
		} else {
			return false;
		}
	}
	return false;
}
