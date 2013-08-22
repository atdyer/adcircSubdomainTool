#include "Project.h"

Project::Project()
{
	fullDomain = 0;
}


Project::Project(QString projFile)
{
	Project();
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


void Project::SetProjectDirectory(QDir newDirectory)
{

}


void Project::SetFullFort14(QDir newPath)
{

}


void Project::SetFullFort15(QDir newPath)
{

}


void Project::SetFullFort63(QDir newPath)
{

}


unsigned int Project::CreateNewSubdomain(QString newName)
{
	return 0;
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
