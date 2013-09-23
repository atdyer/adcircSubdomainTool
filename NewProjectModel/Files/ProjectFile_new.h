#ifndef PROJECT_FILE_NEW_H
#define PROJECT_FILE_NEW_H

#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>

#include <iostream>

class ProjectFile_new : public QDomDocument
{
	public:
		ProjectFile_new();

		/* Open or Create a Project */
		bool	OpenProjectFile_new(QString filePath);
		bool	CreateProjectFile_new(QString parentDirectory, QString projectName);
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
		QString		GetSubDomainBNList(QString subdomainName);
		QString		GetSubDomainFort14(QString subdomainName);
		QString		GetSubDomainFort15(QString subdomainName);
		QString		GetSubDomainFort63(QString subdomainName);
		QString		GetSubDomainFort64(QString subdomainName);
		QString		GetSubDomainPy140(QString subdomainName);
		QString		GetSubDomainPy141(QString subdomainName);
		QString		GetAdcircLocation();
		QDateTime	GetLastFileAccess();

		/* Setter Functions */
		void	SetFullDomainFort10(QString newLoc, bool symLink);
		void	SetFullDomainFort11(QString newLoc, bool symLink);
		void	SetFullDomainFort13(QString newLoc, bool symLink);
		void	SetFullDomainFort14(QString newLoc, bool symLink);
		void	SetFullDomainFort15(QString newLoc, bool symLink);
		void	SetFullDomainFort19(QString newLoc, bool symLink);
		void	SetFullDomainFort20(QString newLoc, bool symLink);
		void	SetFullDomainFort22(QString newLoc, bool symLink);
		void	SetFullDomainFort23(QString newLoc, bool symLink);
		void	SetFullDomainFort24(QString newLoc, bool symLink);
		void	SetFullDomainFort63(QString newLoc, bool symLink);
		void	SetFullDomainFort64(QString newLoc, bool symLink);
		void	SetSubDomainName(QString oldName, QString newName);
		void	SetSubDomainBNList(QString subDomain, QString newLoc);
		void	SetSubDomainFort14(QString subDomain, QString newLoc);
		void	SetSubDomainFort15(QString subDomain, QString newLoc);
		void	SetSubDomainFort63(QString subDomain, QString newLoc);
		void	SetSubDomainFort64(QString subDomain, QString newLoc);
		void	SetSubDomainPy140(QString subDomain, QString newLoc);
		void	SetSubDomainPy141(QString subDomain, QString newLoc);
		void	SetAdcircLocation(QString newLoc);

		/* Adder Functions */
		bool	AddSubdomain(QString newName);


		/* Static tag strings */
		static const QString	TAG_PROJECT;
		static const QString	TAG_FULL_DOMAIN;
		static const QString	TAG_SUB_DOMAIN;
		static const QString	TAG_SETTINGS;

		/* Static attribute strings */
		static const QString	ATTR_NAME;
		static const QString	ATTR_DIRECTORY;
		static const QString	ATTR_BNLISTLOCATION;
		static const QString	ATTR_FORT015LOCATION;
		static const QString	ATTR_FORT063LOCATION;
		static const QString	ATTR_FORT064LOCATION;
		static const QString	ATTR_FORT10LOCATION;
		static const QString	ATTR_FORT11LOCATION;
		static const QString	ATTR_FORT13LOCATION;
		static const QString	ATTR_FORT14LOCATION;
		static const QString	ATTR_FORT15LOCATION;
		static const QString	ATTR_FORT19LOCATION;
		static const QString	ATTR_FORT20LOCATION;
		static const QString	ATTR_FORT22LOCATION;
		static const QString	ATTR_FORT23LOCATION;
		static const QString	ATTR_FORT24LOCATION;
		static const QString	ATTR_FORT63LOCATION;
		static const QString	ATTR_FORT64LOCATION;
		static const QString	ATTR_MAXELELOCATION;
		static const QString	ATTR_MAXVELLOCATION;
		static const QString	ATTR_PY140;
		static const QString	ATTR_PY141;
		static const QString	ATTR_ADCIRCLOCATION;
		static const QString	ATTR_LASTSAVE;

	private:

		QString		projectName;
		QFile		projectFile;
		QDir		projectDirectory;
		QDateTime	lastModified;

		/* File IO Functions */
		void	SetProjectFile_new(QString filePath);
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
		bool	CheckForExistingFile(QString filename);
		bool	CopyFile(QString oldFile, QString newDir);
		bool	LinkFile(QString oldFile, QString newDir);
		void	RemoveFile(QString oldFile);


		/* Dialogs */
		bool	WarnProjectAlreadyOpen();
		void	WarnFileError(QString message);
		bool	WarnFileExists(QString fileLoc);
		bool	WarnSubdomainExists(QString subName);
};

#endif // PROJECTFILE_NEW_H
