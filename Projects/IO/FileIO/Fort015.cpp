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
	ExtractAllBoundaryNodes();
//	ExtractAllOuterBoundaryNodes();
//	if (subdomainApproach == 2)
//	{
//		ExtractAllInnerBoundaryNodes();
//	}
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
			for (std::set<unsigned int>::iterator it = outerBoundaries.begin(); it != outerBoundaries.end(); ++it)
			{
				fort015 << *it << std::endl;
			}
			fort015 << innerBoundaries.size() << "\t!nibnr" << std::endl;
			for (std::set<unsigned int>::iterator it = innerBoundaries.begin(); it != innerBoundaries.end(); ++it)
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


bool Fort015::ExtractAllBoundaryNodes()
{
	std::cout << "Extracting all boundary nodes from " << subDomains.size() << " subdomains" << std::endl;
	innerBoundaries.clear();
	outerBoundaries.clear();
	if (subDomains.size() > 0)
	{
		Domain *currSubdomain = 0;
		for (std::vector<Domain*>::iterator it=subDomains.begin(); it != subDomains.end(); ++it)
		{
			currSubdomain = *it;
			if (currSubdomain)
			{
				Boundaries currBoundaries = boundaryFinder.FindAllBoundaries(currSubdomain->GetAllElements());

				Py140 currPy140 (currSubdomain->GetPy140Location());
				currBoundaries.innerBoundaryNodes = currPy140.ConvertNewToOld(currBoundaries.innerBoundaryNodes);
				currBoundaries.outerBoundaryNodes = currPy140.ConvertNewToOld(currBoundaries.outerBoundaryNodes);

				std::cout << "Found " << currBoundaries.innerBoundaryNodes.size() << " inner boundary nodes and " <<
					     currBoundaries.outerBoundaryNodes.size() << " outer boundary nodes" << std::endl;
				if (currBoundaries.innerBoundaryNodes.size() > 0)
					innerBoundaries.insert(currBoundaries.innerBoundaryNodes.begin(), currBoundaries.innerBoundaryNodes.end());
				if (currBoundaries.outerBoundaryNodes.size() > 0)
					outerBoundaries.insert(currBoundaries.outerBoundaryNodes.begin(), currBoundaries.outerBoundaryNodes.end());
			} else {
				return false;
			}
		}
		if (innerBoundaries.size() == 0)
			return false;
		if (outerBoundaries.size() == 0)
			return false;
	}

	return true;
}
