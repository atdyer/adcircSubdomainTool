#ifndef FORT021_new_H
#define FORT021_new_H

#include <QObject>

#include <fstream>
#include <ostream>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort021_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort021_new(QObject *parent=0);
		Fort021_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

		bool	StartWriting();
		void	WriteHeader(int numTS, std::vector<unsigned int> nodeList);
		void	WriteTimestep(std::vector<std::string> tsData);
		void	FinishedWriting();

	private:

		QString			domainName;
		std::ofstream		file;
		ProjectFile_new*	projectFile;

		QString	GetFilePath();
};
#endif // FORT021_new_H
