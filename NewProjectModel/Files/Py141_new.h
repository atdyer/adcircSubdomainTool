#ifndef PY141_new_H
#define PY141_new_H

#include <QObject>

#include <vector>
#include <map>
#include <istream>
#include <fstream>
#include <sstream>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Py141_new : public QObject
{
		Q_OBJECT
	public:
		explicit Py141_new(QObject *parent=0);
		Py141_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

		void					SaveFile();
		void					SetNewToOld(std::map<unsigned int, unsigned int> newMap);
		void					SetOldToNew(std::map<unsigned int, unsigned int> newMap);

		unsigned int				ConvertNewToOld(unsigned int newNum);
		std::vector<unsigned int>		ConvertNewToOld(std::vector<unsigned int> newVector);
		unsigned int				ConvertOldToNew(unsigned int oldNum);
		std::vector<unsigned int>		ConvertOldToNew(std::vector<unsigned int> oldVector);
		QString					GetFilePath();
		std::map<unsigned int, unsigned int>	GetNewToOld();
		std::map<unsigned int, unsigned int>	GetOldToNew();

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
		QString			targetFile;

		std::map<unsigned int, unsigned int>	newToOldElements;
		std::map<unsigned int, unsigned int>	oldToNewElements;

		void	ReadFile();
};

#endif // PY141_new_H
