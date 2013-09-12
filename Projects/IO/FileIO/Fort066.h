#ifndef FORT066_H
#define FORT066_H

#include <QString>

class Fort066
{
	public:
		Fort066();
		~Fort066();

		void	SetFilePath(QString newLoc);

	private:

		QString	filePath;

};

#endif // FORT066_H
