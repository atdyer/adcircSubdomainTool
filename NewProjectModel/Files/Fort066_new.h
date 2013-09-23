#ifndef FORT066_NEW_H
#define FORT066_NEW_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort066_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort066_new(QObject *parent=0);
		Fort066_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort066_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT066_NEW_H
