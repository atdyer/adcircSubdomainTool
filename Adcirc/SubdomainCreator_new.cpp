#include "SubdomainCreator_new.h"

SubdomainCreator_new::SubdomainCreator_new() :
	bnList(0),
	fullDomain(0),
	py140(0),
	py141(0),
	projectFile(0),
	selectedElements(),
	selectedNodes(),
	subdomainName()
{
}


SubdomainCreator_new::~SubdomainCreator_new()
{
	if (bnList)
		delete bnList;
	if (py140)
		delete py140;
	if (py141)
		delete py141;
}


bool SubdomainCreator_new::CreateSubdomain(QString newName, ProjectFile_new *projFile, FullDomain *fDomain)
{
	fullDomain = fDomain;
	projectFile = projFile;
	subdomainName = newName;

	if (fullDomain && projectFile && projectFile->AddSubdomain(subdomainName))
	{
		// Get elements (and nodes) that are selected in the full domain
		selectedElements = fullDomain->GetSelectedElements();
		FindUniqueNodes();

		// Map the subdomain nodes/elements to the full domain nodes/elements
		MapOldToNewElements();
		MapOldToNewNodes();

		// Get the boundaries of the selected elements
		FindBoundaries();

		// Write new fort.14 file
		return WriteFort14();
	}
	return false;
}


void SubdomainCreator_new::FindBoundaries()
{
	bnList = new BNList14_new(subdomainName, projectFile);

	if (bnList && py140)
	{
		BoundarySearch boundarySearch;
		Boundaries selectedBoundaries = boundarySearch.FindBoundaries(selectedElements);

		std::vector<unsigned int> innerBoundaries;
		std::vector<unsigned int> outerBoundaries;

		for (std::set<unsigned int>::iterator it=selectedBoundaries.innerBoundaryNodes.begin();
		     it != selectedBoundaries.innerBoundaryNodes.end();
		     ++it)
		{
			innerBoundaries.push_back(py140->ConvertOldToNew(*it));
		}

		for (std::set<unsigned int>::iterator it=selectedBoundaries.outerBoundaryNodes.begin();
		     it != selectedBoundaries.outerBoundaryNodes.end();
		     ++it)
		{
			outerBoundaries.push_back(py140->ConvertOldToNew(*it));
		}

		bnList->SetInnerBoundaryNodes(innerBoundaries);
		bnList->SetOuterBoundaryNodes(outerBoundaries);
		bnList->SaveFile();
	}
}


void SubdomainCreator_new::FindUniqueNodes()
{
	std::set<Node*> selectedSet;
	Element *currElement = 0;
	for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
	{
		currElement = *it;
		selectedSet.insert(currElement->n1);
		selectedSet.insert(currElement->n2);
		selectedSet.insert(currElement->n3);
	}

	selectedNodes.reserve(selectedSet.size());
	for (std::set<Node*>::iterator it=selectedSet.begin(); it != selectedSet.end(); ++it)
	{
		selectedNodes.push_back(*it);
	}
}


void SubdomainCreator_new::MapOldToNewElements()
{
	py140 = new Py140_new(subdomainName, projectFile);

	if (selectedElements.size())
	{
		Element *currElement = 0;
		unsigned int nextNewElement = 1;
		std::map<unsigned int, unsigned int> oldToNew;
		for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				oldToNew[currElement->elementNumber] = nextNewElement++;
			}
		}
		py140->SetOldToNew(oldToNew);
		py140->SaveFile();
	}
}


void SubdomainCreator_new::MapOldToNewNodes()
{
	py141 = new Py141_new(subdomainName, projectFile);

	if (selectedNodes.size())
	{
		Node *currNode = 0;
		unsigned int nextNewNode = 1;
		std::map<unsigned int, unsigned int> oldToNew;
		for (std::vector<Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); ++it)
		{
			currNode = *it;
			if (currNode)
			{
				oldToNew[currNode->nodeNumber] = nextNewNode++;
			}
		}
		py141->SetOldToNew(oldToNew);
		py141->SaveFile();
	}
}


bool SubdomainCreator_new::WriteFort14()
{
	QString targetPath = projectFile->GetSubDomainDirectory(subdomainName) + QDir::separator() + "fort.14";
	std::ofstream fort14File (targetPath.toStdString().data());
	if (fort14File.is_open() && py140 && py141)
	{
		// Write title line
		fort14File << subdomainName.toStdString().data() << "\n";

		// Write info line
		fort14File << selectedElements.size() << " " << selectedNodes.size() << "\n";

		// Write nodes
		Node *currNode = 0;
		for (std::vector<Node*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
		{
			currNode = *it;
			if (currNode)
			{
				fort14File << "\t" <<
					      py140->ConvertOldToNew(currNode->nodeNumber) << "\t" <<
					      currNode->xDat << "\t" <<
					      currNode->yDat << "\t" <<
					      currNode->zDat << "\n";
			}
		}

		// Write elements
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				fort14File << py141->ConvertOldToNew(currElement->elementNumber) << "\t3\t" <<
					      py141->ConvertOldToNew(currElement->n1->nodeNumber) << "\t" <<
					      py141->ConvertOldToNew(currElement->n2->nodeNumber) << "\t" <<
					      py141->ConvertOldToNew(currElement->n3->nodeNumber) << "\n";
			}
		}

		// Write boundaries
		fort14File << "0\t!no. of open boundary segments\n";
		fort14File << "0\t!no. of open boundary nodes\n";
		fort14File << "0\t!no. of land boundary segments\n";
		fort14File << "0\t!no. of land boundary nodes\n";

		// Close the file
		fort14File.close();

		projectFile->SetSubDomainFort14(subdomainName, targetPath);

		return true;
	} else {
		return false;
	}
}
