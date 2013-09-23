#ifndef PY141_new_H
#define PY141_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Py141_new : public QObject
{
		Q_OBJECT
	public:
		explicit Py141_new(QObject *parent=0);
		Py141_new(ProjectFile_new *projectFile, QObject *parent=0);
		Py141_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // PY141_new_H
