#ifndef FORT021_new_H
#define FORT021_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort021_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort021_new(QObject *parent=0);
		Fort021_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort021_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};
#endif // FORT021_new_H
