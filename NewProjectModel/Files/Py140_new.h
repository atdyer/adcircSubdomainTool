#ifndef PY140_new_H
#define PY140_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Py140_new : public QObject
{
		Q_OBJECT
	public:
		explicit Py140_new(QObject *parent=0);
		Py140_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // PY140_new_H
