#ifndef MAXVEL63_new_H
#define MAXVEL63_new_H


#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Maxvel63_new : public QObject
{
		Q_OBJECT
	public:
		explicit Maxvel63_new(QObject *parent=0);
		Maxvel63_new(ProjectFile_new *projectFile, QObject *parent=0);
		Maxvel63_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // MAXVEL63_new_H
