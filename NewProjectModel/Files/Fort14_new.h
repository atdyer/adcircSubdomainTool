#ifndef FORT14_new_H
#define FORT14_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort14_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14_new(QObject *parent=0);
		Fort14_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);
		~Fort14_new();

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT14_new_H
