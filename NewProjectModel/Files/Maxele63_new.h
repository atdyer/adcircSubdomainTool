#ifndef MAXELE63_new_H
#define MAXELE63_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Maxele63_new : public QObject
{
		Q_OBJECT
	public:
		explicit Maxele63_new(QObject *parent=0);
		Maxele63_new(ProjectFile_new *projectFile, QObject *parent=0);
		Maxele63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // MAXELE63_new_H
