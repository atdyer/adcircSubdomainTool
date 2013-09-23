#ifndef BNLIST14_new_H
#define BNLIST14_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class BNList14_new : public QObject
{
		Q_OBJECT
	public:
		explicit BNList14_new(QObject *parent=0);
		BNList14_new(ProjectFile_new *projectFile, QObject *parent=0);
		BNList14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // BNLIST14_new_H
