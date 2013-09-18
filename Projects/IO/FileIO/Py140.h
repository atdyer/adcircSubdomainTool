#ifndef PY140_H
#define PY140_H

#include <map>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#include <QString>
#include <QFile>

class Py140
{
	public:
		Py140();
		~Py140();

		void	SetFilePath(QString newPath);
		void	SetOldToNew(std::map<unsigned int, unsigned int> newMap);
		void	SetNewToOld(std::map<unsigned int, unsigned int> newMap);
		void	SetNumFullNodes(int newCount);
		void	ReadFile();
		void	WriteFile();

		std::map<unsigned int, unsigned int>	GetOldToNew();
		std::map<unsigned int, unsigned int>	GetNewToOld();
		std::vector<unsigned int>		ConvertNewToOld(std::vector<unsigned int> newList);
		std::vector<unsigned int>		ConvertOldToNew(std::vector<unsigned int> oldList);
		unsigned int				ConvertNewToOld(unsigned int newNum);
		unsigned int				ConvertOldToNew(unsigned int oldNum);

	private:

		QString filePath;
		int	numFullNodes;

		std::map<unsigned int, unsigned int>	oldToNewNodes;
		std::map<unsigned int, unsigned int>	newToOldNodes;

		void	FillNewToOld();
		void	FillOldToNew();
};

#endif // PY140_H
