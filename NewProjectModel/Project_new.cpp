#include "Project_new.h"

Project_new::Project_new(QObject *parent) :
	QObject(parent)
{
	std::cout << "Created project: " << objectName().toStdString() << std::endl;
	fullDomain = new FullDomain(this);
	subDomain = new SubDomain(this);
}


Project_new::~Project_new()
{
	std::cout << "Deleted project: " << objectName().toStdString() << std::endl;
}
