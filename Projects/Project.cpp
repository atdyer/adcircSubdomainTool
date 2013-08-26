#include "Project.h"

Project::Project() :
	fullDomain(0),
	projectTree(0)
{

}


Project::~Project()
{
	if (fullDomain)
		delete fullDomain;
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
	if (ProjectIsOpen() && !fullDomain)
	{
		fullDomain = new Domain();
		QString fullFort14 = testProjectFile.GetFullDomainFort14();
		if (!fullFort14.isEmpty())
		{
			fullDomain->SetFort14Location(fullFort14.toStdString());
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


Domain* Project::GetFullDomain()
{
	return fullDomain;
}


Domain* Project::GetActiveSubdomain()
{
	return currSubDomain;
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

			QTreeWidgetItem *treeTop = new QTreeWidgetItem(projectTree);
			treeTop->setData(0, Qt::DisplayRole, testProjectFile.GetProjectName());
			treeTop->setData(0, Qt::FontRole, boldFont);
			treeTop->setData(0, Qt::StatusTipRole, testProjectFile.GetProjectName() +
					 " - " +
					 testProjectFile.GetProjectDirectory());

			QTreeWidgetItem *fullDomainTop = new QTreeWidgetItem(treeTop);
			fullDomainTop->setData(0, Qt::DisplayRole, QString("Full Domain"));

			QTreeWidgetItem *subDomainsTop = new QTreeWidgetItem(treeTop);
			subDomainsTop->setData(0, Qt::DisplayRole, QString("Sub Domains"));

			if (fullDomain)
			{
				QString fort14Loc (fullDomain->GetFort14Location().data());
				if (!fort14Loc.isEmpty())
				{
					QTreeWidgetItem *fullDomainFort14 = new QTreeWidgetItem(fullDomainTop);
					fullDomainFort14->setIcon(0, QIcon::fromTheme("text-x-generic"));
					fullDomainFort14->setData(0, Qt::DisplayRole, QString("fort.14"));
					fullDomainFort14->setData(0, Qt::StatusTipRole, fort14Loc);
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
