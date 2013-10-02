#include "Fort14Reader.h"

Fort14Reader::Fort14Reader(QString fileLoc, std::vector<Node> *nodeList, std::vector<Element> *elementList, QObject *parent) :
	QObject(parent),
	elements(elementList),
	nodes(nodeList),
	targetFile(fileLoc)
{

}


Fort14Reader::~Fort14Reader()
{
	std::cout << "Deleting fort.14 reader" << std::endl;
}


void Fort14Reader::ReadFile()
{
	emit StartedReading();

	std::cout << "Reading on thread: " << this->thread() << std::endl;

	for (int i=0; i<100; ++i)
	{
		Node testNode;
		testNode.nodeNumber = i;
		nodes->push_back(testNode);

		Element testElement;
		testElement.elementNumber = i;
		elements->push_back(testElement);

		usleep(10000);
		emit Progress(i);
	}

	emit FinishedReading();
}
