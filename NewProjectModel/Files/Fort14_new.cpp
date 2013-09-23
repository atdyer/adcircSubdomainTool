#include "Fort14_new.h"

/**
 * @brief Default Constructor
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(0)
{

}


/**
 * @brief Constructor to be used if this fort.14 file is for a full domain
 * @param projectFile The project file to get file data from
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	projectFile(projectFile)
{

}


/**
 * @brief Constructor to be used if this fort.14 file is for a subdomain
 * @param domainName The subdomain's name
 * @param projectFile The project file to get file data from
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	projectFile(projectFile)
{

}


Fort14_new::~Fort14_new()
{

}
