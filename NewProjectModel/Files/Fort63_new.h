#ifndef FORT63_new_H
#define FORT63_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort63_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort63_new(QObject *parent=0);
		Fort63_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT63_new_H
