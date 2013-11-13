#include "ProjectFile_new.h"

const QString ProjectFile_new::TAG_FULL_DOMAIN = "fullDomain";
const QString ProjectFile_new::TAG_PROJECT = "adcSubdomainProject";
const QString ProjectFile_new::TAG_SETTINGS = "settings";
const QString ProjectFile_new::TAG_SUB_DOMAIN = "subDomain";

const QString ProjectFile_new::ATTR_ADCIRCLOCATION = "adcircExe";
const QString ProjectFile_new::ATTR_BNLISTLOCATION = "bnListLoc";
const QString ProjectFile_new::ATTR_DIRECTORY = "dir";
const QString ProjectFile_new::ATTR_FORT10LOCATION = "fort10Loc";
const QString ProjectFile_new::ATTR_FORT11LOCATION = "fort11Loc";
const QString ProjectFile_new::ATTR_FORT13LOCATION = "fort13Loc";
const QString ProjectFile_new::ATTR_FORT14LOCATION = "fort14Loc";
const QString ProjectFile_new::ATTR_FORT15LOCATION = "fort15Loc";
const QString ProjectFile_new::ATTR_FORT19LOCATION = "fort19Loc";
const QString ProjectFile_new::ATTR_FORT20LOCATION = "fort20Loc";
const QString ProjectFile_new::ATTR_FORT22LOCATION = "fort22Loc";
const QString ProjectFile_new::ATTR_FORT23LOCATION = "fort23Loc";
const QString ProjectFile_new::ATTR_FORT24LOCATION = "fort24Loc";
const QString ProjectFile_new::ATTR_FORT63LOCATION = "fort63Loc";
const QString ProjectFile_new::ATTR_FORT64LOCATION = "fort64Loc";
const QString ProjectFile_new::ATTR_FORT015LOCATION = "fort015Loc";
const QString ProjectFile_new::ATTR_FORT019LOCATION = "fort019Loc";
const QString ProjectFile_new::ATTR_FORT020LOCATION = "fort020Loc";
const QString ProjectFile_new::ATTR_FORT021LOCATION = "fort021Loc";
const QString ProjectFile_new::ATTR_FORT065LOCATION = "fort065Loc";
const QString ProjectFile_new::ATTR_FORT066LOCATION = "fort066Loc";
const QString ProjectFile_new::ATTR_FORT067LOCATION = "fort067Loc";
const QString ProjectFile_new::ATTR_LASTSAVE = "savedOn";
const QString ProjectFile_new::ATTR_MAXELELOCATION = "maxeleLoc";
const QString ProjectFile_new::ATTR_MAXVELLOCATION = "maxvelLoc";
const QString ProjectFile_new::ATTR_NAME = "name";
const QString ProjectFile_new::ATTR_PY140 = "py140Loc";
const QString ProjectFile_new::ATTR_PY141 = "py141Loc";


ProjectFile_new::ProjectFile_new()
{
	FileModified();
}


/**
 * @brief Opens an Adcirc Subdomain Project file
 *
 * Once the file has been successfully opened, all ProjectFile_new access functions
 * become available.
 *
 * Opens an Adcirc Subdomain Project file. If a project is already open,
 * we prompt the user to either save it, discard changes, or cancel opening
 * a new project.
 *
 * @param filePath The project file to open
 */
bool ProjectFile_new::OpenProjectFile_new(QString filePath)
{
	if (!ProjectIsOpen() || WarnProjectAlreadyOpen())
	{
		SetProjectFile_new(filePath);
		OpenFileRead();
		ReadFile();
		CloseFile();
		return true;
	}
	return false;
}


/**
 * @brief Creates a project directory (and a project file within that directory) in the
 * parent directory
 *
 * Creates a project directory (and a project file within that directory) in the
 * parent directory. The newly created project file is opened and all ProjectFile_new
 * access functions become available.
 *
 * If a project file is already open, we first prompt the user to either save,
 * discard changes, or cancel creating a new project.
 *
 * @param parentDirectory The directory in which to create the project directory
 * @param projectName The name to give both the project directory and the project file
 */
bool ProjectFile_new::CreateProjectFile_new(QString parentDirectory, QString projectName)
{
	if (!ProjectIsOpen() || WarnProjectAlreadyOpen())
	{
		/* Move into the project directory */
		/* If the directory doesn't exist, try to create it */
		projectDirectory = QDir(parentDirectory);
		if (!projectDirectory.cd(projectName))
		{
			if (!projectDirectory.mkdir(projectName))
			{
				WarnFileError("Unable to create project directory");
				return false;
			}
			else if (!projectDirectory.cd(projectName))
			{
				WarnFileError("Unable to access the project directory");
				return false;
			}
		}

		/* Make sure there isn't a project file in the directory already */
		projectDirectory.setNameFilters(QStringList("*.spf"));
		if (!projectDirectory.entryList().isEmpty())
		{
			WarnFileError(QString("A project file already exists at ").append(parentDirectory+projectName));
			return false;
		}

		/* Create the file */
		SetProjectFile_new(projectDirectory.absolutePath() + QDir::separator() + projectName + ".spf");
		if (OpenFileWrite())
		{
			/* Create an empty project */
			CreateEmptyProject();

			/* Save the project file */
			SaveFile();

			/* Close the project file */
			CloseFile();

			return true;
		}
	}
	return false;
}


bool ProjectFile_new::SaveProject()
{
	if (OpenFileWrite())
		if (SaveFile())
			return CloseFile();
	return false;
}


bool ProjectFile_new::ProjectIsOpen()
{
	return documentElement().hasAttributes() || documentElement().hasChildNodes();
}


QString ProjectFile_new::GetAdcircLocation()
{
	return GetAttribute(TAG_SETTINGS, ATTR_ADCIRCLOCATION);
}


QString ProjectFile_new::GetFullDomainDirectory()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_DIRECTORY);
}


QString ProjectFile_new::GetFullDomainFort14()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT14LOCATION);
}


QString ProjectFile_new::GetFullDomainFort15()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT15LOCATION);
}


QString ProjectFile_new::GetFullDomainFort63()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT63LOCATION);
}


QString ProjectFile_new::GetFullDomainFort64()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT64LOCATION);
}


QString ProjectFile_new::GetFullDomainFort015()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT015LOCATION);
}


QString ProjectFile_new::GetFullDomainFort065()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT065LOCATION);
}


QString ProjectFile_new::GetFullDomainFort066()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT066LOCATION);
}


QString ProjectFile_new::GetFullDomainFort067()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_FORT067LOCATION);
}


QString ProjectFile_new::GetFullDomainMaxele()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_MAXELELOCATION);
}


QString ProjectFile_new::GetFullDomainMaxvel()
{
	return GetAttribute(TAG_FULL_DOMAIN, ATTR_MAXVELLOCATION);
}


QDateTime ProjectFile_new::GetLastFileAccess()
{
	return lastModified;
}


QString ProjectFile_new::GetProjectDirectory()
{
	return projectDirectory.absolutePath();
}


QString ProjectFile_new::GetProjectName()
{
	return projectName;
}


QString ProjectFile_new::GetSubDomainBNList(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_BNLISTLOCATION);
}


QString ProjectFile_new::GetSubDomainDirectory(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_DIRECTORY);
}


QString ProjectFile_new::GetSubDomainFort14(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT14LOCATION);
}


QString ProjectFile_new::GetSubDomainFort15(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT15LOCATION);
}


QString ProjectFile_new::GetSubDomainFort63(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT63LOCATION);
}


QString ProjectFile_new::GetSubDomainFort64(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT64LOCATION);
}


QString ProjectFile_new::GetSubDomainFort015(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT015LOCATION);
}


QString ProjectFile_new::GetSubDomainFort019(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT019LOCATION);
}


QString ProjectFile_new::GetSubDomainFort020(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT020LOCATION);
}


QString ProjectFile_new::GetSubDomainFort021(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_FORT021LOCATION);
}


QString ProjectFile_new::GetSubDomainMaxele(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_MAXELELOCATION);
}


QString ProjectFile_new::GetSubDomainMaxvel(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_MAXVELLOCATION);
}


QStringList ProjectFile_new::GetSubDomainNames()
{
	QStringList subdomainNames;

	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement subdomainNameAttribute = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (!subdomainNameAttribute.isNull())
		{
			subdomainNames.append(subdomainNameAttribute.text());
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}

	return subdomainNames;
}


QString ProjectFile_new::GetSubDomainPy140(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_PY140);
}


QString ProjectFile_new::GetSubDomainPy141(QString subdomainName)
{
	return GetAttributeSubdomain(subdomainName, ATTR_PY141);
}


void ProjectFile_new::SetAdcircLocation(QString newLoc)
{
	SetAttribute(TAG_SETTINGS, ATTR_ADCIRCLOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainDirectory(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_DIRECTORY, newLoc);
}


void ProjectFile_new::SetFullDomainFort10(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT10LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort11(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT11LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort13(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT13LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort14(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT14LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort15(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT15LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort19(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT19LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort20(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT20LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort22(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT22LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort23(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT23LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort24(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT24LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort63(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT63LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort64(QString newLoc, bool symLink)
{
	if (symLink ? LinkFile(newLoc, projectDirectory.absolutePath()) : CopyFile(newLoc, projectDirectory.absolutePath()))
		SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT64LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort015(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT015LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort065(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT065LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort066(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT066LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainFort067(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_FORT067LOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainMaxele(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_MAXELELOCATION, newLoc);
}


void ProjectFile_new::SetFullDomainMaxvel(QString newLoc)
{
	SetAttribute(TAG_FULL_DOMAIN, ATTR_MAXVELLOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainBNList(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_BNLISTLOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainDirectory(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_DIRECTORY, newLoc);
}


void ProjectFile_new::SetSubDomainFort14(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT14LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort15(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT15LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort63(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT63LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort64(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT64LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort015(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT015LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort019(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT019LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort020(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT020LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainFort021(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_FORT021LOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainMaxele(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_MAXELELOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainMaxvel(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_MAXVELLOCATION, newLoc);
}


void ProjectFile_new::SetSubDomainName(QString oldName, QString newName)
{
	SetAttributeSubdomain(oldName, ATTR_NAME, newName);
}


void ProjectFile_new::SetSubDomainPy140(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_PY140, newLoc);
}


void ProjectFile_new::SetSubDomainPy141(QString subDomain, QString newLoc)
{
	SetAttributeSubdomain(subDomain, ATTR_PY141, newLoc);
}


bool ProjectFile_new::AddSubdomain(QString newName)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == newName)
		{
			if (!WarnSubdomainExists(newName))
			{
				return false;
			} else {
				documentElement().removeChild(currentSubdomain);
				break;
			}
		}

		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
//		{
//			QDomNode attributeNode = currentSubdomain.namedItem(attribute);
//			if (!attributeNode.isNull())
//			{
//				currentSubdomain.removeChild(attributeNode);
//			}

//			QDomElement attributeElement = createElement(attribute);
//			attributeElement.appendChild(createTextNode(value));
//			currentSubdomain.appendChild(attributeElement);
//			break;
//		}
	}

	QDomElement newSubdomain = createElement(TAG_SUB_DOMAIN);
	QDomElement newSubdomainName = createElement(ATTR_NAME);
	documentElement().appendChild(newSubdomain);
	newSubdomain.appendChild(newSubdomainName);
	newSubdomainName.appendChild(createTextNode(newName));

	SaveProject();

	return true;
}


void ProjectFile_new::SetProjectFile_new(QString filePath)
{
	projectFile.setFileName(filePath);
	projectDirectory = QFileInfo(filePath).absoluteDir();
	projectName = QFileInfo(filePath).baseName();
}


bool ProjectFile_new::OpenFileRead()
{
	if (!projectFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		std::cout << projectFile.fileName().toStdString().data() << std::endl;
		WarnFileError(QString("Unable to open file ").append(projectFile.fileName()));
		return false;
	}

	return true;
}


bool ProjectFile_new::OpenFileWrite()
{
	if (!projectFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		std::cout << projectFile.fileName().toStdString().data() << std::endl;
		WarnFileError(QString("Unable to open file ").append(projectFile.fileName()));
		return false;
	}

	return true;
}


bool ProjectFile_new::ReadFile()
{

	if (!setContent(&projectFile))
	{
		WarnFileError("Error setting project data");
		projectFile.close();
		return false;
	}

	FileModified();
	return true;
}


bool ProjectFile_new::SaveFile()
{
	FileModified();
	QString projectText = toString(5);
	QTextStream fileOut(&projectFile);
	fileOut << projectText;
	projectFile.close();
	return true;
}


bool ProjectFile_new::CloseFile()
{
	projectFile.close();
	return true;
}


void ProjectFile_new::CreateEmptyProject()
{
	clear();
	appendChild(createElement(TAG_PROJECT));
	QDomElement fullDomainElement = createElement(TAG_FULL_DOMAIN);
	QDomElement subDomainElement = createElement(TAG_SUB_DOMAIN);
	QDomElement settingsElement = createElement(TAG_SETTINGS);
	documentElement().appendChild(fullDomainElement);
	documentElement().appendChild(subDomainElement);
	documentElement().appendChild(settingsElement);
}


QString ProjectFile_new::GetAttribute(QString attribute)
{
	QDomElement attributeElement = documentElement().namedItem(attribute).toElement();
	if (!attributeElement.isNull())
	{
		return attributeElement.text();
	}
	return QString();
}


QString ProjectFile_new::GetAttribute(QString tag, QString attribute)
{
	QDomElement tagElement = documentElement().namedItem(tag).toElement();
	if (!tagElement.isNull())
	{
		QDomElement attributeElement = tagElement.namedItem(attribute).toElement();
		if (!attributeElement.isNull())
		{
			return attributeElement.text();
		}
	}
	return QString();
}


QString ProjectFile_new::GetAttribute(QString parentTag, QString childTag, QString attribute)
{
	QDomElement parentTagElement = documentElement().namedItem(parentTag).toElement();
	if (!parentTagElement.isNull())
	{
		QDomElement childTagElement = parentTagElement.namedItem(childTag).toElement();
		if (!childTagElement.isNull())
		{
			QDomElement attributeElement = childTagElement.namedItem(attribute).toElement();
			if (!attributeElement.isNull())
			{
				return attributeElement.text();
			}
		}
	}
	return QString();
}


QString ProjectFile_new::GetAttributeSubdomain(QString subdomainName, QString attribute)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == subdomainName)
		{
			QDomElement attributeElement = currentSubdomain.namedItem(attribute).toElement();
			if (!attributeElement.isNull())
			{
				return attributeElement.text();
			}
			break;
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}
	return QString();
}


void ProjectFile_new::SetAttribute(QString attribute, QString value)
{
	QDomNode attributeNode = documentElement().namedItem(attribute);
	if (!attributeNode.isNull())
	{
		documentElement().removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	documentElement().appendChild(attributeElement);

	SaveProject();
}


void ProjectFile_new::SetAttribute(QString tag, QString attribute, QString value)
{
	QDomElement tagElement = documentElement().namedItem(tag).toElement();
	if (tagElement.isNull())
	{
		tagElement = createElement(tag);
		documentElement().appendChild(tagElement);
	}

	QDomNode attributeNode = tagElement.namedItem(attribute);
	if (!attribute.isNull())
	{
		tagElement.removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	tagElement.appendChild(attributeElement);

	SaveProject();
}


void ProjectFile_new::SetAttribute(QString parentTag, QString childTag, QString attribute, QString value)
{
	QDomElement parentTagElement = documentElement().namedItem(parentTag).toElement();
	if (parentTagElement.isNull())
	{
		parentTagElement = createElement(parentTag);
		documentElement().appendChild(parentTagElement);
	}

	QDomElement childTagElement = parentTagElement.namedItem(childTag).toElement();
	if (childTagElement.isNull())
	{
		childTagElement = createElement(childTag);
		parentTagElement.appendChild(childTagElement);
	}

	QDomNode attributeNode = childTagElement.namedItem(value);
	if (!attributeNode.isNull())
	{
		childTagElement.removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(attribute);
	attributeElement.appendChild(createTextNode(value));
	childTagElement.appendChild(attributeElement);

	SaveProject();
}


void ProjectFile_new::SetAttributeSubdomain(QString subdomainName, QString attribute, QString value)
{
	QDomElement currentSubdomain = documentElement().firstChildElement(TAG_SUB_DOMAIN);
	while (!currentSubdomain.isNull())
	{
		QDomElement nameElement = currentSubdomain.namedItem(ATTR_NAME).toElement();
		if (nameElement.text() == subdomainName)
		{
			QDomNode attributeNode = currentSubdomain.namedItem(attribute);
			if (!attributeNode.isNull())
			{
				currentSubdomain.removeChild(attributeNode);
			}

			QDomElement attributeElement = createElement(attribute);
			attributeElement.appendChild(createTextNode(value));
			currentSubdomain.appendChild(attributeElement);
			break;
		}
		currentSubdomain = currentSubdomain.nextSiblingElement(TAG_SUB_DOMAIN);
	}

	SaveProject();
}


void ProjectFile_new::FileModified()
{
	lastModified = QDateTime::currentDateTime();

	QDomNode attributeNode = documentElement().namedItem(ATTR_LASTSAVE);
	if (!attributeNode.isNull())
	{
		documentElement().removeChild(attributeNode);
	}

	QDomElement attributeElement = createElement(ATTR_LASTSAVE);
	attributeElement.appendChild(createTextNode(lastModified.toString(Qt::ISODate)));
	documentElement().appendChild(attributeElement);
}


bool ProjectFile_new::CheckForExistingFile(QString filename)
{
	return QFile(filename).exists();
}


bool ProjectFile_new::CopyFile(QString oldFile, QString newDir)
{
	QString newFile = newDir + QDir::separator() + QFileInfo(oldFile).fileName();

	if (CheckForExistingFile(newFile))
	{
		if (WarnFileExists(newFile))
		{
			RemoveFile(newFile);
		} else {
			return false;
		}
	}

	QFile file (oldFile);
	if (file.exists())
	{
		return file.copy(newFile);
	}
	return false;
}


bool ProjectFile_new::LinkFile(QString oldFile, QString newDir)
{
	QString newLink = newDir + QDir::separator() + QFileInfo(oldFile).fileName();

#ifdef Q_OS_WIN32
	newLinkName.append(".lnk");
#endif

	if (CheckForExistingFile(newLink))
	{
		if (WarnFileExists(newLink))
		{
			RemoveFile(newLink);
		} else {
			return false;
		}
	}

	QFile file (oldFile);
	if (file.exists())
	{
		return file.link(newLink);
	}
	return false;
}


void ProjectFile_new::RemoveFile(QString oldFile)
{
	QDir().remove(oldFile);
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
bool ProjectFile_new::WarnProjectAlreadyOpen()
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
void ProjectFile_new::WarnFileError(QString message)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText(QString("File Error: ").append(message));
	msgBox.setStandardButtons(QMessageBox::Ok);
	msgBox.exec();
}


bool ProjectFile_new::WarnFileExists(QString fileLoc)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("The file " + fileLoc + " already exists. Would you like to overwrite it?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	switch (msgBox.exec())
	{
		case QMessageBox::Yes:
			/* Overwrite the file */
			return true;
		case QMessageBox::No:
			/* Do not overwrite the file */
			return false;
		default:
			/* Should never be reached */
			return false;
	}
}


bool ProjectFile_new::WarnSubdomainExists(QString subName)
{
	QMessageBox msgBox;
	msgBox.setWindowTitle("Adcirc Subdomain Modeling Tool");
	msgBox.setText("The subdomain " + subName + " already exists. Would you like to overwrite it?");
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);

	switch (msgBox.exec())
	{
		case QMessageBox::Yes:
			/* Overwrite the file */
			return true;
		case QMessageBox::No:
			/* Do not overwrite the file */
			return false;
		default:
			/* Should never be reached */
			return false;
	}
}
