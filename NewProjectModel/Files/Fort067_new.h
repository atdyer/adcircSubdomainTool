#ifndef FORT067_NEW_H
#define FORT067_NEW_H

#include <map>
#include <fstream>
#include <istream>
#include <sstream>

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort067_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort067_new(QObject *parent=0);
		Fort067_new(ProjectFile_new *projectFile, QObject *parent=0);

		bool					StartReading();
		bool					HasNextTimestep();
		std::map<unsigned int, std::string>	GetNextTimestep();
		int					GetNumTimesteps();
		void					FinishedReading();

	private:

		std::map<unsigned int, std::string>	currentData;
		int					currTS;
		std::ifstream				file;
		int					numNodes;
		int					numTS;
		ProjectFile_new*			projectFile;


		void	SearchForFile();
};

#endif // FORT067_NEW_H
