#include "Project.h"

Project::Project() :
	fullDomain(0),
	currentDomain(0),
	projectTree(0),
	progressBar(0)
{

}


Project::~Project()
{
	if (fullDomain)
		delete fullDomain;
	if (subDomains.size() != 0)
	{
		for (std::map<QString, Domain*>::iterator it=subDomains.begin(); it != subDomains.end(); ++it)
		{
			Domain *currDomain = it->second;
			delete currDomain;
		}
	}
}


/**
 * @brief Sets the tree widget that is used to display project contents
 *
 * Sets the tree widget that is used to display project contents.
 *
 * @param newTree The project tree of the UI
 */
void Project::SetProjectTree(QTreeWidget *newTree)
{
	projectTree = newTree;
	ConnectProjectTree();
}


void Project::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;
}


void Project::CreateProject()
{
	CreateProjectDialog dialog;
	dialog.setModal(true);
	if (dialog.exec())
	{
		testProjectFile.CreateProjectFile(dialog.GetProjectDirectory(), dialog.GetProjectName());
	}
}


void Project::PopulateFromProjectFile()
{
	if (ProjectIsOpen())
	{
		if (!fullDomain)
		{
			fullDomain = new Domain();
			if (progressBar)
			{
				fullDomain->SetProgressBar(progressBar);
			}
			QString fullFort14 = testProjectFile.GetFullDomainFort14();
			if (!fullFort14.isEmpty())
			{
				fullDomain->SetFort14Location(fullFort14);
			}
		}

		QStringList subdomainNames = testProjectFile.GetSubDomainNames();
		for (int i=0; i<subdomainNames.count(); ++i)
		{
			QString currName = subdomainNames.at(i);
			if (subDomains.count(currName) == 0)
			{
				Domain *newSubdomain = new Domain();
				if (progressBar)
				{
					newSubdomain->SetProgressBar(progressBar);
				}
				subDomains[currName] = newSubdomain;
				QString subFort14 = testProjectFile.GetSubDomainFort14(currName);
				if (!subFort14.isEmpty())
				{
					newSubdomain->SetFort14Location(subFort14);
				}
			}
		}
	}
}


void Project::OpenProject()
{
	QStringList selections;
	QFileDialog dialog(0, "Open an ADCIRC Subdomain Project", QDir::homePath());
	dialog.setModal(true);
	dialog.setNameFilter("ADCIRC Subdomain Projects (*.spf)");
	dialog.setFileMode(QFileDialog::ExistingFile);
	if (dialog.exec())
	{
		selections = dialog.selectedFiles();
		if (!selections.isEmpty())
		{
			testProjectFile.OpenProjectFile(selections.first());
			PopulateFromProjectFile();
			UpdateTreeDisplay();
		}
	}
}


bool Project::ProjectIsOpen()
{
	return testProjectFile.ProjectIsOpen();
}


void Project::SetFullFort14(QString newPath)
{

}


void Project::SetFullFort15(QString newPath)
{

}


void Project::SetFullFort63(QString newPath)
{

}


unsigned int Project::CreateNewSubdomain(QString newName)
{
	return 0;
}


Domain* Project::GetActiveDomain()
{
	return currentDomain;
}


void Project::ConnectProjectTree()
{
	if (projectTree)
	{
		connect(projectTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			this, SLOT(on_ProjectTreeItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
	}
}


void Project::UpdateTreeDisplay()
{
	if (projectTree)
	{
		projectTree->clear();
		if (ProjectIsOpen())
		{
			QFont boldFont;
			boldFont.setBold(true);

			/* The top of the tree */
			QTreeWidgetItem *treeTop = new QTreeWidgetItem(projectTree);
			treeTop->setData(0, Qt::DisplayRole, testProjectFile.GetProjectName());
			treeTop->setData(0, Qt::FontRole, boldFont);
			treeTop->setData(0, Qt::StatusTipRole, testProjectFile.GetProjectName() +
					 " - " +
					 testProjectFile.GetProjectDirectory());

			/* The full domain branch */
			QTreeWidgetItem *fullDomainTop = new QTreeWidgetItem(treeTop);
			fullDomainTop->setData(0, Qt::DisplayRole, QString("Full Domain"));

			/* The subdomain branch */
			QTreeWidgetItem *subDomainsTop = new QTreeWidgetItem(treeTop);
			subDomainsTop->setData(0, Qt::DisplayRole, QString("Sub Domains"));

			/* Populate full domain branch */
			if (fullDomain)
			{
				QString fort14Loc  = fullDomain->GetFort14Location();
				if (!fort14Loc.isEmpty())
				{
					QTreeWidgetItem *fullDomainFort14 = new QTreeWidgetItem(fullDomainTop);
					fullDomainFort14->setIcon(0, QIcon::fromTheme("text-x-generic"));
					fullDomainFort14->setData(0, Qt::DisplayRole, QString("fort.14"));
					fullDomainFort14->setData(0, Qt::StatusTipRole, fort14Loc);
				}
			}

			/* Create subdomain branches */
			if (subDomains.size() > 0)
			{
				for (std::map<QString, Domain*>::iterator it = subDomains.begin(); it != subDomains.end(); ++it)
				{
					QString domainName = it->first;
					Domain* currDomain = it->second;

					/* Create the subdomain branch */
					QTreeWidgetItem *currDomainTop = new QTreeWidgetItem(subDomainsTop);
					currDomainTop->setData(0, Qt::DisplayRole, domainName);

					/* Populate the branch with data from the subdomain */
					QString fort14Loc = currDomain->GetFort14Location();
					if (!fort14Loc.isEmpty())
					{
						QTreeWidgetItem *currSubFort14 = new QTreeWidgetItem(currDomainTop);
						currSubFort14->setIcon(0, QIcon::fromTheme("text-x-generic"));
						currSubFort14->setData(0, Qt::DisplayRole, QString("fort.14"));
						currSubFort14->setData(0, Qt::StatusTipRole, fort14Loc);
					}
				}
			}

			projectTree->expandAll();
		}
	}
}


void Project::CreateFullDomain()
{
	if (!fullDomain)
	{
		fullDomain = new Domain();
	}
}


void Project::SetCurrentDomain(Domain *domain)
{
	if (domain)
	{
		currentDomain = domain;
		emit newDomainSelected();
	}
}


Domain* Project::DetermineSelectedDomain(QTreeWidgetItem *item)
{
	QString itemText = item->data(0, Qt::DisplayRole).toString();
	if (itemText == QString("Full Domain"))
	{
		return fullDomain;
	}
	QTreeWidgetItem *parentItem = item->parent();
	if (parentItem)
	{
		QString parentText = parentItem->data(0, Qt::DisplayRole).toString();
		if (parentText == QString("Full Domain"))
		{
			return fullDomain;
		}
		else if (parentText == QString("Sub Domains"))
		{
			if (subDomains.count(itemText) != 0)
			{
				return subDomains[itemText];
			}
		} else {
			return DetermineSelectedDomain(parentItem);
		}
	}
	return 0;
}


void Project::on_ProjectTreeItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *)
{
	Domain *selectedDomain = DetermineSelectedDomain(item);
	if (selectedDomain && selectedDomain != currentDomain)
	{
		SetCurrentDomain(selectedDomain);
	}
}
