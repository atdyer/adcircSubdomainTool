#ifndef FORT020_new_H
#define FORT020_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort020_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort020_new(QObject *parent=0);
		Fort020_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT020_new_H
