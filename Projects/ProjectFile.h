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
		bool	ProjectIsOpen();

		/* Getter Functions */
		QString		GetProjectName();
		QString		GetProjectDirectory();
		QString		GetFullDomainFort14();
		QString		GetFullDomainFort15();
		QString		GetFullDomainFort63();
		QString		GetFullDomainFort64();
		QStringList	GetSubDomainNames();
		QString		GetSubDomainFort14(QString subdomainName);
		QString		GetSubDomainFort15(QString subdomainName);
		QString		GetSubDomainFort63(QString subdomainName);
		QString		GetSubDomainFort64(QString subdomainName);
		QString		GetAdcircLocation();
		QDateTime	GetLastFileAccess();

		/* Setter Functions */
		void	SetFullDomainFort14(QString newLoc);
		void	SetFullDomainFort15(QString newLoc);
		void	SetFullDomainFort63(QString newLoc);
		void	SetFullDomainFort64(QString newLoc);
		void	SetSubDomainName(QString oldName, QString newName);
		void	SetSubDomainFort14(QString subDomain, QString newLoc);
		void	SetSubDomainFort15(QString subDomain, QString newLoc);
		void	SetSubDomainFort63(QString subDomain, QString newLoc);
		void	SetSubDomainFort64(QString subDomain, QString newLoc);
		void	SetAdcircLocation(QString newLoc);


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

		/* File IO Functions */
		void	SetProjectFile(QString filePath);
		bool	OpenFileRead();
		bool	OpenFileWrite();
		bool	ReadFile();
		bool	SaveFile();
		bool	CloseFile();


		/* DOM Functions */
		void	CreateEmptyProject();
		QString GetAttribute(QString attribute);
		QString	GetAttribute(QString tag, QString attribute);
		QString GetAttribute(QString parentTag, QString childTag, QString attribute);
		QString	GetAttributeSubdomain(QString subdomainName, QString attribute);

		void	SetAttribute(QString attribute, QString value);
		void	SetAttribute(QString tag, QString attribute, QString value);
		void	SetAttribute(QString parentTag, QString childTag, QString attribute, QString value);
		void	SetAttributeSubdomain(QString subdomainName, QString attribute, QString value);


		/* File Write Functions */
		void	FileModified();


		/* Dialogs */
		bool	WarnProjectAlreadyOpen();
		void	WarnFileError(QString message);
};

#endif // PROJECTFILE_H
