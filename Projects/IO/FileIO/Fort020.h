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

		void	WriteInfoLines(QString allLines);
		void	WriteTimestep(QString allLines);

	private:




};

#endif // FORT020_H
