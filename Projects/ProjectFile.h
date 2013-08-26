#ifndef PROJECTFILE_H
#define PROJECTFILE_H

#include <QDomDocument>
#include <QFile>
#include <QMessageBox>

class ProjectFile : public QDomDocument
{
	public:
		ProjectFile();
		ProjectFile(QString filePath);

		/* Getter functions */
		QString	GetFullDomainFort14();
		QString GetFullDomainFort15();
		QString	GetFullDomainFort63();
		QString	GetFullDomainFort64();
		QString	GetSubDomainFort14(QString subdomainName);
		QString	GetSubDomainFort15(QString subdomainName);
		QString	GetSubDomainFort63(QString subdomainName);
		QString	GetSubDomainFort64(QString subdomainName);


		/* Setter functions */

	private:

		bool	fileOpen;

		QDomNode	fullDomainNode;
		QDomNodeList	subDomainNodes;


		/* File read functions */
		bool	OpenFile(QString filePath);
		bool	IsValidProjectFile();
		QString	GetFullDomainAttribute(QString attributeName);
		QString	GetSubDomainAttribute(QString subdomainName, QString attributeName);
		QString	GetAttribute(QDomElement element, QString attributeName);


		/* File write functions */


		/* Dialogs */
		void	WarnFileError(QString message);
};

#endif // PROJECTFILE_H
