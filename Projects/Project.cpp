#include "Project.h"

Project::Project(QWidget *parent) :
	parentWidget(parent),
	fullDomain(0)
{
	fullDomain = 0;
}


Project::Project(QString projFile, QWidget *parent) :
	parentWidget(parent),
	projectFile(projFile),
	fullDomain(0)
{
	ReadProjectFile(projFile);
}


Project::~Project()
{
//	if (fullDomain)
//		delete fullDomain;

//	for (unsigned int i=0; i<subDomains.size(); ++i)
//		if (subDomains[i])
//			delete subDomains[i];
}


bool Project::CreateProject()
{
	/* Open up the create project dialog */

}


/**
 * @brief Creates a project in the directory if one does not already exist
 *
 * Creates a project in the directory if one does not already exist.
 *
 * @param directory The directory in which to create the new project
 * @param filename The name to give the new project file (without extension)
 * @return true if the project was successfully created
 * @return false if the project was not created
 */
bool Project::CreateProject(QString directory, QString projectName)
{
	projectDir = directory + projectName;

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
	CreateProjectFile(projectDir, projectName);
	return true;
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


/**
 * @brief Creates a project file in the provided directory if one does not already exist
 *
 * Creates a project file in the provided directory if one does not already exist. If one
 * does exist, this function warns the user before returning.
 *
 * @param directory The directory in which to create the new project file
 * @param filename The name to give the new project file (without extension)
 */
void Project::CreateProjectFile(QString directory, QString filename)
{

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
 * @brief Warns the user that we were unable to create the desired directory
 *
 * Warns the user that we were unable to create the desired directory.
 *
 * @param directory The directory that we tried to create
 */
void Project::WarnUnableToCreateDir(QString directory)
{
	QMessageBox::warning(parentWidget,
			     "Adcirc Subdomain Modeling Tool",
			     "Unable to create directory:\n" + directory,
			     QMessageBox::Ok);
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
	QMessageBox::warning(parentWidget,
			     "Adcirc Subdomain Modeling Tool",
			     "A project file already exists in the following directory:\n" + directory,
			     QMessageBox::Ok);
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
