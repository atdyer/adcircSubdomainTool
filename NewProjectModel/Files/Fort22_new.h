#ifndef FORT22_new_H
#define FORT22_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort22_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort22_new(QObject *parent=0);
		Fort22_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort22_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT22_new_H
