#include "ProjectFile.h"

ProjectFile::ProjectFile()
{
	fileOpen = false;
	FileModified();
}


/**
 * @brief Opens an Adcirc Subdomain Project file
 *
 * Once the file has been successfully opened, all ProjectFile access functions
 * become available.
 *
 * Opens an Adcirc Subdomain Project file. If a project is already open,
 * we prompt the user to either save it, discard changes, or cancel opening
 * a new project.
 *
 * @param filePath The project file to open
 */
void ProjectFile::OpenProjectFile(QString filePath)
{
	if (!fileOpen || WarnProjectAlreadyOpen())
	{
		fileOpen = OpenFile(filePath);
	}
}


/**
 * @brief Creates a project directory (and a project file within that directory) in the
 * parent directory
 *
 * Creates a project directory (and a project file within that directory) in the
 * parent directory. The newly created project file is opened and all ProjectFile
 * access functions become available.
 *
 * If a project file is already open, we first prompt the user to either save,
 * discard changes, or cancel creating a new project.
 *
 * @param parentDirectory The directory in which to create the project directory
 * @param projectName The name to give both the project directory and the project file
 */
void ProjectFile::CreateProjectFile(QString parentDirectory, QString projectName)
{
	if (!fileOpen || WarnProjectAlreadyOpen())
	{
		/* Move into the project directory */
		/* If the directory doesn't exist, try to create it */
		projectDirectory = QDir(parentDirectory);
		if (!projectDirectory.cd(projectName))
		{
			if (!projectDirectory.mkdir(projectName))
			{
				WarnFileError("Unable to create project directory");
				return;
			}
			else if (!projectDirectory.cd(projectName))
			{
				WarnFileError("Unable to access the project directory");
				return;
			}
		}

		/* Make sure there isn't a project file in the directory already */
		projectDirectory.setNameFilters(QStringList("*.spf"));
		if (!projectDirectory.entryList().isEmpty())
		{
			WarnFileError(QString("A project file already exists at ").append(parentDirectory+projectName));
			return;
		}

		/* Create the file */
		QString filePath = projectDirectory.absolutePath() + QDir::separator() + projectName + ".spf";
		projectFile.setFileName(filePath);
		if (!projectFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			WarnFileError(QString("Unable to open project file ").append(filePath));
			return;
		}

		/* Write an empty project to the file */
		QString emptyProject ("<adcSubdomainProject>\n</adcSubdomainProject>\n");
		QTextStream fileOut(&projectFile);
		fileOut << emptyProject;
		projectFile.close();

		/* Open the new project to make it available for use */
		fileOpen = OpenFile(filePath);
	}
}


bool ProjectFile::ProjectIsOpen()
{
	return fileOpen;
}


QString ProjectFile::GetProjectName()
{
	return projectName;
}


QString ProjectFile::GetProjectDirectory()
{
	return projectDirectory.absolutePath();
}


QString ProjectFile::GetFullDomainFort14()
{
	return GetFullDomainAttribute("fort14Loc");
}


QString ProjectFile::GetFullDomainFort15()
{
	return GetFullDomainAttribute("fort15Loc");
}


QString ProjectFile::GetFullDomainFort63()
{
	return GetFullDomainAttribute("fort63Loc");
}


QString ProjectFile::GetFullDomainFort64()
{
	return GetFullDomainAttribute("fort64Loc");
}


QString ProjectFile::GetSubDomainFort14(QString subdomainName)
{
	return GetSubDomainAttribute(subdomainName, "fort14Loc");
}


QString ProjectFile::GetSubDomainFort15(QString subdomainName)
{
	return GetSubDomainAttribute(subdomainName, "fort15Loc");
}


QString ProjectFile::GetSubDomainFort63(QString subdomainName)
{
	return GetSubDomainAttribute(subdomainName, "fort63Loc");
}


QString ProjectFile::GetSubDomainFort64(QString subdomainName)
{
	return GetSubDomainAttribute(subdomainName, "fort64Loc");
}


QString ProjectFile::GetADCIRCLocation()
{
	return GetSettingsAttribute("adcircExe");
}


QStringList ProjectFile::GetSubDomainNames()
{
	QStringList subdomainNames;

	if (!subDomainNodes.isEmpty())
	{
		for(int i=0; i<subDomainNodes.count(); ++i)
		{
			if (subDomainNodes.at(i).isElement())
			{
				QDomElement subDomainElement = subDomainNodes.at(i).toElement();
				subdomainNames.append(GetAttribute(subDomainElement, QString("name")));
			}
		}
	}

	return subdomainNames;
}


QDateTime ProjectFile::GetLastFileAccess()
{
	return lastModified;
}


bool ProjectFile::OpenFile(QString filePath)
{
	projectFile.setFileName(filePath);
	projectDirectory = QFileInfo(filePath).absoluteDir();
	projectName = QFileInfo(filePath).baseName();

	if (!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		WarnFileError(QString("Unable to open file ").append(filePath));
		return false;
	}

	if (!setContent(&projectFile))
	{
		WarnFileError("Error setting project data");
		projectFile.close();
		return false;
	}
	projectFile.close();

	if (IsValidProjectFile())
	{
		QDomNodeList fullDomainNodes = documentElement().elementsByTagName("fullDomain");
		if (fullDomainNodes.count() > 1)
		{
			WarnFileError("More than one full domain is defined in the project file. Using the first defined");
		}
		else if (!fullDomainNodes.isEmpty())
		{
			fullDomainNode = fullDomainNodes.at(0);
		}

		subDomainNodes = documentElement().elementsByTagName("subDomain");

		QDomNodeList settingsNodes = documentElement().elementsByTagName("settings");
		if (settingsNodes.count() > 1)
		{
			WarnFileError("More than one settings block is defined in the project file. Using the first block");
		}
		else if (!settingsNodes.isEmpty())
		{
			settingsNode = settingsNodes.at(0);
		}

	} else {
		return false;
	}

	FileModified();

	return true;
}


/**
 * @brief Determines if the current DOM object represents a valid
 * subdomain project file
 *
 * Determines if the current DOM object represents a valid
 * subdomain project file.
 *
 * Things to check:
 * - Is the document tag "adcSubdomainProject"?
 * - Do all subdomains have unique names?
 *
 * @return true if it is a valid project file
 * @return false if it is not a valid project file
 */
bool ProjectFile::IsValidProjectFile()
{
	return documentElement().tagName() == "adcSubdomainProject";
}


/**
 * @brief Searches through the Full Domain QDomNode for the given attribute name
 *
 * Searches through the Full Domain QDomNode for the given attribute name.
 *
 * @param attributeName The attribute to search for
 * @return The value of the requested attribute, if it was found
 * @return An empty string if the attribute could not be found
 */
QString ProjectFile::GetFullDomainAttribute(QString attributeName)
{
	if (fileOpen)
	{
		if (fullDomainNode.isElement())
		{
			return GetAttribute(fullDomainNode.toElement(), attributeName);
		}
	}

	return QString();
}


/**
 * @brief Searches for the given attribute in the given subdomain
 *
 * Searches for the given attribute in the given subdomain.
 *
 * @param subdomainName The name of the subdomain to search
 * @param attributeName The name of the desired attribute
 * @return The value of the attribute, if it exists
 * @return An empty QString if the subdomain or attribute does not exist
 */
QString ProjectFile::GetSubDomainAttribute(QString subdomainName, QString attributeName)
{
	if (fileOpen)
	{
		if (!subDomainNodes.isEmpty())
		{
			for (int i=0; i < subDomainNodes.count(); ++i)
			{
				if (subDomainNodes.at(i).isElement())
				{
					QDomElement subDomainElement = subDomainNodes.at(i).toElement();
					if (GetAttribute(subDomainElement, "name") == subdomainName)
					{
						return GetAttribute(subDomainElement, attributeName);
					}
				}
			}
		}
	}

	return QString();
}


QString ProjectFile::GetSettingsAttribute(QString attributeName)
{
	if (fileOpen)
	{
		return GetAttribute(settingsNode.toElement(), attributeName);
	}
	return QString();
}


/**
 * @brief Searches for an attribute in any given QDomElement
 *
 * Searches for an attribute in any given QDomElement.
 *
 * @param element The QDomElement to search
 * @param attributeName The attribute to search for
 * @return The value of the attribute if it is found
 * @return An empty QString if the attribute is not found
 */
QString ProjectFile::GetAttribute(QDomElement element, QString attributeName)
{
	QDomNode currentAttribute = element.firstChild();
	while (!currentAttribute.isNull())
	{
		if (currentAttribute.isElement())
		{
			QDomElement attributeElement = currentAttribute.toElement();
			if (attributeElement.tagName() == attributeName)
			{
				return attributeElement.text();
			}
		}
		currentAttribute = currentAttribute.nextSibling();
	}
	return QString();
}


void ProjectFile::FileModified()
{
	lastModified = QDateTime::currentDateTime();
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
bool ProjectFile::WarnProjectAlreadyOpen()
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
 * @brief Warns the user that an error occurred when reading a file
 *
 * Warns the user that an error occurred when reading a file. The message
 * is displayed, prepended with "File Error: "
 *
 * @param message The error message to display
 */
void ProjectFile::WarnFileError(QString message)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText(QString("File Error: ").append(message));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}
