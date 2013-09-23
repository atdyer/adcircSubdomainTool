#ifndef FORT067_NEW_H
#define FORT067_NEW_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort067_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort067_new(QObject *parent=0);
		Fort067_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort067_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT067_NEW_H
