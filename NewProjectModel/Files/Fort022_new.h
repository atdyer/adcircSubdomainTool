#ifndef FORT022_new_H
#define FORT022_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort022_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort022_new(QObject *parent=0);
		Fort022_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT022_new_H
