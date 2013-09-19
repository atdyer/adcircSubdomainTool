#ifndef FORT020_H
#define FORT020_H

#include <QString>

class Fort020
{
	public:

		Fort020();
		Fort020(QString newLoc);
		~Fort020();

		void	SetFilePath(QString newLoc);

		void	WriteInfoLines(std::string allLines);
		void	WriteTimestep(std::string allLines);

	private:




};

#endif // FORT020_H
