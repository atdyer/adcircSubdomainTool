#ifndef FORT015_NEW_H
#define FORT015_NEW_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort015_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort015_new(QObject *parent=0);
		Fort015_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT015_NEW_H
