#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>

#include <iostream>


class ProjectFile : public QDomDocument
{
	public:
		ProjectFile();

		/* Open or Create a Project */
		bool	OpenProjectFile(QString filePath);
		bool	CreateProjectFile(QString parentDirectory, QString projectName);
		bool	SaveProject();

		/* Getter Functions */
		bool	ProjectIsOpen();
		QString GetProjectName();
		QString GetProjectDirectory();
		QString	GetFullDomainFort14();
		QString GetFullDomainFort15();
		QString	GetFullDomainFort63();
		QString	GetFullDomainFort64();
		QString	GetSubDomainFort14(QString subdomainName);
		QString	GetSubDomainFort15(QString subdomainName);
		QString	GetSubDomainFort63(QString subdomainName);
		QString	GetSubDomainFort64(QString subdomainName);
		QString	GetAdcircLocation();
		QStringList	GetSubDomainNames();
		QDateTime	GetLastFileAccess();

		/* Setter Functions */


		/* Static tag strings */
		static const QString	TAG_PROJECT;
		static const QString	TAG_FULL_DOMAIN;
		static const QString	TAG_SUB_DOMAIN;
		static const QString	TAG_SETTINGS;

		/* Static attribute strings */
		static const QString	ATTR_NAME;
		static const QString	ATTR_DIRECTORY;
		static const QString	ATTR_FORT015LOCATION;
		static const QString	ATTR_FORT063LOCATION;
		static const QString	ATTR_FORT064LOCATION;
		static const QString	ATTR_FORT14LOCATION;
		static const QString	ATTR_FORT15LOCATION;
		static const QString	ATTR_FORT63LOCATION;
		static const QString	ATTR_FORT64LOCATION;
		static const QString	ATTR_MAXELELOCATION;
		static const QString	ATTR_MAXVELLOCATION;
		static const QString	ATTR_ADCIRCLOCATION;
		static const QString	ATTR_LASTSAVE;

	private:

		QString		projectName;
		QFile		projectFile;
		QDir		projectDirectory;
		QDateTime	lastModified;

//		QDomNode	fullDomainNode;
//		QDomNodeList	subDomainNodes;
//		QDomNode	settingsNode;
//		QDomElement	rootElement;

		/* File IO Functions */
		void	SetProjectFile(QString filePath);
		bool	OpenFileRead();
		bool	OpenFileWrite();
		bool	ReadFile();
		bool	SaveFile();
		bool	CloseFile();
//		bool	IsValidProjectFile();


		/* DOM Functions */
		void	CreateEmptyProject();
//		QString	GetFullDomainAttribute(QString attributeName);
//		QString	GetSubDomainAttribute(QString subdomainName, QString attributeName);
//		QString	GetSettingsAttribute(QString attributeName);
//		QString	GetAttribute(QDomElement element, QString attributeName);

		QString GetAttribute(QString attribute);
		QString	GetAttribute(QString tag, QString attribute);
		QString GetAttribute(QString parentTag, QString childTag, QString attribute);
		QString	GetAttributeSubdomain(QString subdomainName, QString attribute);


		/* File Write Functions */
		void	FileModified();



		/* Dialogs */
		bool	WarnProjectAlreadyOpen();
		void	WarnFileError(QString message);
};

#endif // PROJECTFILE_H
