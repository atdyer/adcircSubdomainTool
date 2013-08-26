#include "ProjectFile.h"

ProjectFile::ProjectFile()
{
	fileOpen = false;
}


ProjectFile::ProjectFile(QString filePath)
{
	fileOpen = OpenFile(filePath);
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


bool ProjectFile::OpenFile(QString filePath)
{
	QFile file (filePath);

	if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		WarnFileError(QString("Unable to open file ").append(filePath));
		return false;
	}

	if (!setContent(&file))
	{
		WarnFileError("Error setting project data");
		file.close();
		return false;
	}
	file.close();

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

	} else {
		return false;
	}

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
					if (subDomainElement.tagName() == subdomainName)
					{
						return GetAttribute(subDomainElement, attributeName);
					}
				}
			}
		}
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
