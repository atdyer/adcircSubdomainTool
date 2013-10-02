#ifndef FORT14READER_H
#define FORT14READER_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

#include <QObject>

#include "adcData.h"
#include "Quadtree/Quadtree.h"

class Fort14Reader : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14Reader(QString fileLoc,
				      std::vector<Node> *nodeList,
				      std::vector<Element> *elementList,
				      std::vector<unsigned int> *boundaryList,
				      Quadtree *quadtree,
				      bool normalize,
				      QObject *parent = 0);
		~Fort14Reader();

	signals:

		void	StartedReading();
		void	Progress(int);
		void	FoundDomainBounds(float, float, float, float, float, float);
		void	FinishedReading();
		
	public slots:

		void	ReadFile();

	private:

		std::vector<unsigned int>	boundaryNodes;
		int				currProgress;
		std::vector<Element>*		elements;
		int				fullProgress;
		std::vector<Node>*		nodes;
		bool				normalizeCoordinates;
		Quadtree*			quadtree;
		QString				targetFile;

		Node*	GetNode(unsigned int nodeNumber);
		void	NormalizeCoordinates();
		void	PopulateQuadtree();
		void	ReadBoundaries(std::ifstream *fileStream);
		bool	ReadElementalData(int numElements, std::ifstream *fileStream);
		bool	ReadNodalData(int numNodes, std::ifstream *fileStream);



		
};

#endif // FORT14READER_H
