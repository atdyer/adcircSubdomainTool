#ifndef FORT14READER_H
#define FORT14READER_H

#include <iostream>
#include <vector>

#include <QObject>

#include "adcData.h"

class Fort14Reader : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14Reader(QString fileLoc, std::vector<Node> *nodeList, std::vector<Element> *elementList, QObject *parent = 0);
		~Fort14Reader();

	signals:

		void	StartedReading();
		void	Progress(int);
		void	FinishedReading();
		
	public slots:

		void	ReadFile();

	private:

		std::vector<Element>*	elements;
		std::vector<Node>*	nodes;
		QString			targetFile;

		
};

#endif // FORT14READER_H
