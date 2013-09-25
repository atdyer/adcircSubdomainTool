#ifndef FORT020_new_H
#define FORT020_new_H

#include <QObject>

#include <fstream>
#include <ostream>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort020_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort020_new(QObject *parent=0);
		Fort020_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

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

#endif // FORT020_new_H
