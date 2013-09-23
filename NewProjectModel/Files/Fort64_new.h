#ifndef FORT64_new_H
#define FORT64_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort64_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort64_new(QObject *parent=0);
		Fort64_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort64_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT64_new_H
