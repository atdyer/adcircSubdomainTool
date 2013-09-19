#ifndef PY141_H
#define PY141_H

#include <map>
#include <vector>
#include <set>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QFile>
#include <QString>

class Py141
{
	public:
		Py141();
		Py141(QString py141Path);
		~Py141();

		void	SetFilePath(QString newPath);
		void	SetOldToNew(std::map<unsigned int, unsigned int> newMap);
		void	SetNewToOld(std::map<unsigned int, unsigned int> newMap);
		void	SetNumFullElements(int newCount);
		void	ReadFile();
		void	WriteFile();

		std::map<unsigned int, unsigned int>	GetOldToNew();
		std::map<unsigned int, unsigned int>	GetNewToOld();
		std::vector<unsigned int>		ConvertNewToOld(std::vector<unsigned int> newList);
		std::vector<unsigned int>		ConvertOldToNew(std::vector<unsigned int> oldList);
		std::set<unsigned int>			ConvertNewToOld(std::set<unsigned int> newSet);
		std::set<unsigned int>			ConvertOldToNew(std::set<unsigned int> oldSet);
		unsigned int				ConvertNewToOld(unsigned int newNum);
		unsigned int				ConvertOldToNew(unsigned int oldNum);

	private:

		QString filePath;
		int	numFullElements;

		std::map<unsigned int, unsigned int>	oldToNewElements;
		std::map<unsigned int, unsigned int>	newToOldElements;

		void	FillNewToOld();
		void	FillOldToNew();
};

#endif // PY141_H
