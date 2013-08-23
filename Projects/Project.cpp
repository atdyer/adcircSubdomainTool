#include "Project.h"

Project::Project() :
	fullDomain(0),
	projectTree(0),
	projectOpen(false)
{

}


Project::~Project()
{

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


bool Project::CreateProject()
{
	if (!projectOpen || WarnProjectAlreadyOpen())
	{
		/* Open up the create project dialog */
		CreateProjectDialog dialog;
		dialog.setModal(true);
		if (dialog.exec())
		{
			QString directory = dialog.GetProjectDirectory();
			QString name = dialog.GetProjectName();
			return CreateProject(directory, name);
		}
	}

	return false;
}


/**
 * @brief Creates a project in the directory if one does not already exist
 *
 * Creates a project in the directory if one does not already exist. A directory
 * of the project name is created inside of the provided parent directory, as
 * well as a project file (.spf)
 *
 * @param directory The directory in which to create the new project
 * @param filename The name to give the new project file (without extension)
 * @return true if the project was successfully created
 * @return false if the project was not created
 */
bool Project::CreateProject(QString directory, QString projName)
{
	if (!projectOpen || WarnProjectAlreadyOpen())
	{
		projectDir = directory + projName + QDir::separator();
		projectFile = projName + ".spf";
		projectName = projName;

		/* If the directory doesn't exist, try to create it */
		if (!QDir(projectDir).exists())
		{
			if (!QDir().mkdir(projectDir))
			{
				WarnUnableToCreateDir(projectDir);
				return false;
			}
		}

		/* Make sure there isn't already a project in the directory */
		if (ProjectFileExists(projectDir))
		{
			WarnProjectAlreadyExists(projectDir);
			return false;
		}

		/* Create the new project file */
		return CreateProjectFile(projectDir, projectFile);
	}
	return false;
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


void Project::UpdateTreeDisplay()
{
	if (projectTree)
	{
		projectTree->clear();
		if (projectOpen)
		{
			QFont boldFont;
			boldFont.setBold(true);

			QTreeWidgetItem *treeTop = new QTreeWidgetItem(projectTree);
			treeTop->setData(0, Qt::DisplayRole, projectName);
			treeTop->setData(0, Qt::FontRole, boldFont);
			treeTop->setData(0, Qt::StatusTipRole, projectName + " - " + projectDir);

			QTreeWidgetItem *fullDomainTop = new QTreeWidgetItem(treeTop);
			fullDomainTop->setData(0, Qt::DisplayRole, QString("Full Domain"));

			QTreeWidgetItem *subDomainsTop = new QTreeWidgetItem(treeTop);
			subDomainsTop->setData(0, Qt::DisplayRole, QString("Sub Domains"));

			projectTree->expandAll();
		}
	}
}


/**
 * @brief Creates a project file in the provided directory if one does not already exist
 *
 * Creates a project file in the provided directory if one does not already exist. If one
 * does exist, this function warns the user before returning.
 *
 * @param directory The directory in which to create the new project file
 * @param filename The name to give the new project file (without extension)
 * @return true if the file was successfully created
 * @return false if the file could not be created
 */
bool Project::CreateProjectFile(QString directory, QString filename)
{
	QFile file (directory+filename);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		WarnUnableToCreateFile(filename);
		return false;
	}

	QString emptyProject ("<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n<adcSubdomainProject>\n</adcSubdomainProject>\n");
	QTextStream fileOut(&file);
	fileOut << emptyProject;
	file.close();

	projectOpen = true;

	UpdateTreeDisplay();

	return true;
}


void Project::ReadProjectFile(QString filePath)
{
	QFile file(filePath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		DEBUG("Couldn't Open Project File");
		return;
	}

	QDomDocument document("currProjectTree");
	if (!document.setContent(&file))
	{
		DEBUG("Error parsing project file");
		file.close();
		return;
	}
	file.close();

	QDomElement element = document.documentElement();
	QString rootTag = element.tagName();
	if (rootTag == "adcSubdomainProject")
	{
		/* Read full domain info */
		ReadFullDomainInfo(element.elementsByTagName("fullDomain"));

		/* Read sub domain info */
		ReadSubDomainInfo(element.elementsByTagName("subDomain"));
	}
}


void Project::ReadFullDomainInfo(QDomNodeList nodeList)
{
	if (nodeList.count() != 1)
	{
		DEBUG("Error: More than one full domain defined in project file");
		return;
	}

	QDomElement fullDomainElement = nodeList.at(0).toElement();
	QDomNode currNode = fullDomainElement.firstChild();
	while (!currNode.isNull())
	{
		QDomElement dat = currNode.toElement();
		QString tagName = dat.tagName();
		DEBUG("Full Domain Attribute: " + tagName.toStdString() + " - " + dat.text().toStdString());

		currNode = currNode.nextSibling();
	}
}


void Project::ReadSubDomainInfo(QDomNodeList nodeList)
{
	for (int i=0; i<nodeList.count(); ++i)
	{
		QDomElement subDomainElement = nodeList.at(i).toElement();
		QDomNode currNode = subDomainElement.firstChild();
		while(!currNode.isNull())
		{
			QDomElement dat = currNode.toElement();
			QString tagName = dat.tagName();
			DEBUG(subDomainElement.attribute("id").toStdString() + " - " + tagName.toStdString() + " - " + dat.text().toStdString());

			currNode = currNode.nextSibling();
		}
	}
}


/**
 * @brief Checks if a project file exists in the provided directory
 *
 * Checks if a project file exists in the provided directory.
 *
 * @param checkDirectory The directory to check
 * @return true if a project file exists in the directory
 * @return false if a project file does not exist in the directory
 */
bool Project::ProjectFileExists(QString checkDirectory)
{
	QStringList filters ("*.spf");
	QDir dir (checkDirectory);
	dir.setNameFilters(filters);

	return !dir.entryList().isEmpty();
}


/**
 * @brief Warns the user that a project is currently open and asks if they would like to
 * continue with or without saving
 *
 * Warns the user that a project is currently open and asks if they would like to
 * continue with or without saving. If they choose to continue with saving, the
 * saving will be performed here.
 *
 * @return true if the user would like to continue
 * @return false if the user would not like to continue
 */
bool Project::WarnProjectAlreadyOpen()
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("Project already open. Would you like to save and close the current project?");
	msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel | QMessageBox::Discard);

	switch (msgBox.exec())
	{
		case QMessageBox::Save:
			/* Save and close the project */
			return true;
		case QMessageBox::Cancel:
			/* Return to the current project */
			return false;
		case QMessageBox::Discard:
			/* Close the project without saving */
			return true;
		default:
			/* Should never be reached */
			return false;
	}
}


/**
 * @brief Warns the user that we were unable to create the desired directory
 *
 * Warns the user that we were unable to create the desired directory.
 *
 * @param directory The directory that we tried to create
 */
void Project::WarnUnableToCreateDir(QString directory)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("Unable to create directory:\n" + directory);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}


/**
 * @brief Warns the user that we were unable to create the desired file
 *
 * Warns the user that we were unable to create the desired file.
 *
 * @param filename The file we couldn't create
 */
void Project::WarnUnableToCreateFile(QString filename)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("Unable to create file: " + filename);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}


/**
 * @brief Warns the user that a project file already exists in the directory
 *
 * Warns the user that a project file already exists in the directory.
 *
 * @param directory The directory in which a project file already exists
 *
 */
void Project::WarnProjectAlreadyExists(QString directory)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("A project file already exists in the following directory:\n" + directory);
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}
