#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QDomDocument>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>

class ProjectFile : public QDomDocument
{
	public:
		ProjectFile();

		/* Open or Create a Project */
		void	OpenProjectFile(QString filePath);
		void	CreateProjectFile(QString parentDirectory, QString projectName);

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
		QString	GetADCIRCLocation();
		QStringList	GetSubDomainNames();
		QDateTime	GetLastFileAccess();

		/* Setter Functions */

	private:

		bool	fileOpen;

		QString		projectName;
		QFile		projectFile;
		QDir		projectDirectory;
		QDateTime	lastModified;

		QDomNode	fullDomainNode;
		QDomNodeList	subDomainNodes;
		QDomNode	settingsNode;


		/* File Read Functions */
		bool	OpenFile(QString filePath);
		bool	IsValidProjectFile();
		QString	GetFullDomainAttribute(QString attributeName);
		QString	GetSubDomainAttribute(QString subdomainName, QString attributeName);
		QString	GetSettingsAttribute(QString attributeName);
		QString	GetAttribute(QDomElement element, QString attributeName);


		/* File Write Functions */
		void	FileModified();



		/* Dialogs */
		bool	WarnProjectAlreadyOpen();
		void	WarnFileError(QString message);
};

#endif // PROJECTFILE_H
