#ifndef FORT15_new_H
#define FORT15_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort15_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort15_new(QObject *parent=0);
		Fort15_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort15_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT15_new_H
