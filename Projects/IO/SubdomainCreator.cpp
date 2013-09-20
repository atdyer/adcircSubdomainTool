#include "SubdomainCreator.h"

SubdomainCreator::SubdomainCreator()
{
	currentSelectedState = 0;
	fullNumNodes = 0;
	fullNumElements = 0;

	fort14Path = "";
	bnListPath = "";
	py140Path = "";
	py141Path = "";
}


SubdomainCreator::~SubdomainCreator()
{

}


bool SubdomainCreator::CreateSubdomain()
{
	GetAllRequiredData();
	if (PerformDataValidation())
	{
		if (!WriteFort14File())
		{
			FileWriteError("fort.14");
			return false;
		}
		if (!WriteBNListFile())
		{
			FileWriteError("bnlist.14");
			return false;
		}
		if (!WritePy140File())
		{
			FileWriteError("py.140");
			return false;
		}
		if (!WritePy141File())
		{
			FileWriteError("py.141");
			return false;
		}
		return true;
	}
	return false;
}


void SubdomainCreator::SetDomain(Domain *newDomain)
{
	if (newDomain)
	{
		currentSelectedState = newDomain->GetCurrentSelectedElements();
		fullNumNodes = newDomain->GetNumNodesDomain();
		fullNumElements = newDomain->GetNumElementsDomain();
	}
}


void SubdomainCreator::SetProjectPath(QString newProjectPath)
{
	projectPath = newProjectPath;
}


void SubdomainCreator::SetSubdomainName(QString newName)
{
	subdomainName = newName;
}


QString SubdomainCreator::GetSubdomainName()
{
	return subdomainName;
}


QString SubdomainCreator::GetFort14Location()
{
	return fort14Path;
}


QString SubdomainCreator::GetBNListLocation()
{
	return bnListPath;
}


QString SubdomainCreator::GetPy140Location()
{
	return py140Path;
}


QString SubdomainCreator::GetPy141Location()
{
	return py141Path;
}


void SubdomainCreator::GetAllRequiredData()
{
	FindUniqueNodes();
	FindBoundaryNodes();
	MapOldToNewNodes();
	MapOldToNewElements();
}


bool SubdomainCreator::PerformDataValidation()
{
	if (TestForValidPath())
	{
		if (TestForSufficientElements())
		{
			if (TestForSufficientNodes())
			{
				return TestForValidBoundary();
			}
		}
	}
	return false;
}


bool SubdomainCreator::WriteFort14File()
{
	// Open the file
	fort14Path = targetPath;
	fort14Path.append(QDir::separator()).append("fort.14");
	std::ofstream fort14File;
	fort14File.open(fort14Path.toStdString().data());
	if (fort14File.is_open())
	{
		// Write title line
		fort14File << subdomainName.toStdString().data() << std::endl;

		// Write info line
		fort14File << selectedElements.size() << " " << selectedNodes.size() << std::endl;

		// Write nodes
		fort14File << std::setprecision(12);
		Node *currNode = 0;
		for (std::vector<Node*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
		{
			currNode = *it;
			if (currNode)
			{
				fort14File << "\t" <<
					      oldToNewNodes[currNode->nodeNumber] << "\t" <<
					      currNode->xDat << "\t" <<
					      currNode->yDat << "\t" <<
					      currNode->zDat <<
					      std::endl;
			}
		}

		// Write elements
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			if (currElement)
			{
				fort14File << oldToNewElements[currElement->elementNumber] << "\t3\t" <<
					      oldToNewNodes[currElement->n1->nodeNumber] << "\t" <<
					      oldToNewNodes[currElement->n2->nodeNumber] << "\t" <<
					      oldToNewNodes[currElement->n3->nodeNumber] <<
					      std::endl;
			}
		}

		// Write boundaries
		fort14File << "1\t!no. of open boundary segments" << std::endl;
		fort14File << boundaryNodes.size()-1 << "\t!no. of open boundary nodes" << std::endl;
		fort14File << boundaryNodes.size()-1 << std::endl;
		for (std::vector<unsigned int>::iterator it = boundaryNodes.begin(); it != boundaryNodes.end(); ++it)
		{
			fort14File << oldToNewNodes[*it] << std::endl;
		}
		fort14File << "0\t!no. of land boundary segments" << std::endl;
		fort14File << "0\t!no. of land boundary nodes" << std::endl;

		// Close the file
		fort14File.close();

		return true;
	} else {
		return false;
	}
}


bool SubdomainCreator::WriteBNListFile()
{
//	bnListPath = targetPath;
//	bnListPath.append(QDir::separator()).append("bnlist.14");
//	BNList14 bnList;
//	bnList.SetFilePath(bnListPath);
//	bnList.SetInnerBoundaryNodes();
	return true;
}


bool SubdomainCreator::WritePy140File()
{
	py140Path = targetPath;
	py140Path.append(QDir::separator()).append("py.140");
	std::ofstream py140;
	py140.open(py140Path.toStdString().data());
	if (py140.is_open())
	{
		py140 << "new old " << fullNumNodes << std::endl;
		if (oldToNewNodes.size() != 0)
		{
			for (std::map<unsigned int, unsigned int>::iterator it = oldToNewNodes.begin(); it != oldToNewNodes.end(); ++it)
			{
				py140 << it->second << " " << it->first << std::endl;
			}
		}
		py140.close();
		return true;
	} else {
		std::cout << "Unable to open " << targetPath.append(QDir::separator()).append("py.140").toStdString().data() << std::endl;
		return false;
	}
}


bool SubdomainCreator::WritePy141File()
{
	py141Path = targetPath;
	py141Path.append(QDir::separator()).append("py.141");
	std::ofstream py141;
	py141.open(py141Path.toStdString().data());
	if (py141.is_open())
	{
		py141 << "new old " << fullNumElements << std::endl;
		if (oldToNewElements.size() != 0)
		{
			for (std::map<unsigned int, unsigned int>::iterator it = oldToNewElements.begin(); it != oldToNewElements.end(); ++it)
			{
				py141 << it->second << " " << it->first << std::endl;
			}
		}
		py141.close();
		return true;
	} else {
		return false;
	}
}


void SubdomainCreator::FindUniqueNodes()
{
	if (currentSelectedState)
	{
		// The list of all selected Elements
		selectedElements = *currentSelectedState->GetState();

		// Make a list of all selected Nodes, include duplicates
		selectedNodes.reserve(selectedElements.size()*3);
		Element *currElement = 0;
		for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
		{
			currElement = *it;
			selectedNodes.push_back(currElement->n1);
			selectedNodes.push_back(currElement->n2);
			selectedNodes.push_back(currElement->n3);
		}

		// Now remove duplicates
		std::sort(selectedNodes.begin(), selectedNodes.end());
		std::vector<Node*>::iterator it = std::unique(selectedNodes.begin(), selectedNodes.end());
		selectedNodes.resize(std::distance(selectedNodes.begin(), it));
	}
}


void SubdomainCreator::FindBoundaryNodes()
{
	if (currentSelectedState)
	{
		boundaryNodes = boundaryFinder.FindBoundaries(currentSelectedState);
	}
}


void SubdomainCreator::MapOldToNewNodes()
{
	std::vector<unsigned int> skippedNodes;
	Node *currNode = 0;
	unsigned int newNodeNumber = 1;
	for (std::vector<Node*>::iterator it = selectedNodes.begin(); it != selectedNodes.end(); ++it)
	{
		currNode = *it;
		if (currNode)
		{
			if (oldToNewNodes.count(currNode->nodeNumber) != 0)
			{
				skippedNodes.push_back(currNode->nodeNumber);
			} else {
				oldToNewNodes[currNode->nodeNumber] = newNodeNumber;
				++newNodeNumber;
			}
		}
	}

	if (skippedNodes.size() != 0)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Create Subdomain");
		QString warningText = "Warning: The following duplicate nodes have been removed:\n";
		for (std::vector<unsigned int>::iterator it = skippedNodes.begin(); it != skippedNodes.end(); ++it)
		{
			warningText.append(QString::number(*it).append(", "));
		}
		warningText.chop(2);
		msgBox.setText(warningText);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}


void SubdomainCreator::MapOldToNewElements()
{
	std::vector<unsigned int> skippedElements;
	Element *currElement = 0;
	unsigned int newElementNumber = 1;
	for (std::vector<Element*>::iterator it = selectedElements.begin(); it != selectedElements.end(); ++it)
	{
		currElement = *it;
		if (currElement)
		{
			if (oldToNewElements.count(currElement->elementNumber) != 0)
			{
				skippedElements.push_back(currElement->elementNumber);
			} else {
				oldToNewElements[currElement->elementNumber] = newElementNumber;
				++newElementNumber;
			}
		}
	}

	if (skippedElements.size() != 0)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Create Subdomain");
		QString warningText = "Warning: The following duplicate elements have been removed:\n";
		for (std::vector<unsigned int>::iterator it = skippedElements.begin(); it != skippedElements.end(); ++it)
		{
			warningText.append(QString::number(*it).append(", "));
		}
		warningText.chop(2);
		msgBox.setText(warningText);
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
	}
}


bool SubdomainCreator::TestForValidPath()
{
	QDir projectDir (projectPath);
	if (!projectDir.exists())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Create Subdomain");
		msgBox.setText("Unable to find valid path to the current project");
		msgBox.setIcon(QMessageBox::Critical);
		msgBox.setStandardButtons(QMessageBox::Ok);
		msgBox.exec();
		return false;
	}

	if (subdomainName.isEmpty())
	{
		bool ok;
		subdomainName = QInputDialog::getText(0,
						      "Create Subdomain",
						      "Please enter a name for the new subdomain:",
						      QLineEdit::Normal,
						      "",
						      &ok);
		if (!ok || subdomainName.isEmpty())
		{
			return false;
		}
	}

	QDir subDir (projectPath + QDir::separator() + subdomainName);
	if (subDir.exists())
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle("Create Subdomain");
		msgBox.setText("Warning: The subdomain directory already exists. Any subdomain files currently"
			       "in the directory will be overwritten");
		msgBox.setIcon(QMessageBox::Warning);
		msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
		switch (msgBox.exec())
		{
			case QMessageBox::Ok:
				targetPath = subDir.absolutePath();
				return true;
			case QMessageBox::Cancel:
				return false;
			default:
				return false;
		}
	} else {
		if (projectDir.mkdir(subdomainName))
		{
			targetPath = subDir.absolutePath();
			return true;
		} else {
			return false;
		}
	}
}


bool SubdomainCreator::TestForSufficientElements()
{
	if (selectedElements.size() > 0)
		return true;
	return false;
}


bool SubdomainCreator::TestForSufficientNodes()
{
	if (selectedNodes.size() > 2)
		return true;
	return false;
}


bool SubdomainCreator::TestForValidBoundary()
{
	if (boundaryNodes.size() > 2 && boundaryNodes[0] == boundaryNodes[boundaryNodes.size()-1])
		return true;

	QMessageBox msgBox;
	msgBox.setWindowTitle("Create Subdomain");
	msgBox.setText("Invalid boundary");
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
	return false;
}


void SubdomainCreator::FileWriteError(QString fileName)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Create Subdomain");
	msgBox.setText(fileName.prepend("Error writing file: "));
	msgBox.setIcon(QMessageBox::Critical);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}

