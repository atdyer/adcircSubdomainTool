#include "Fort14_new.h"

/**
 * @brief Default Constructor
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(QObject *parent) :
	QObject(parent),
	domainName(),
	elements(),
	nodes(),
	projectFile(0),
	quadtree(0)
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
	elements(),
	nodes(),
	projectFile(projectFile),
	quadtree(0)
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
	elements(),
	nodes(),
	projectFile(projectFile),
	quadtree(0)
{

}


Fort14_new::~Fort14_new()
{
	if (quadtree)
		delete quadtree;
}


void Fort14_new::Draw()
{

}


std::vector<Element>* Fort14_new::GetElements()
{

}


ShaderType Fort14_new::GetFillShaderType()
{

}


QGradientStops Fort14_new::GetGradientBoundaryColors()
{

}


QGradientStops Fort14_new::GetGradientFillColors()
{

}


QGradientStops Fort14_new::GetGradientOutlineColors()
{

}


float Fort14_new::GetMaxX()
{

}


float Fort14_new::GetMaxY()
{

}


float Fort14_new::GetMaxZ()
{

}


float Fort14_new::GetMinX()
{

}


float Fort14_new::GetMinY()
{

}


float Fort14_new::GetMinZ()
{

}


ShaderType Fort14_new::GetOutlineShaderType()
{

}


float Fort14_new::GetUnprojectedX(float x)
{

}


float Fort14_new::GetUnprojectedY(float y)
{

}


std::vector<Element*>	Fort14_new::GetSelectedElements()
{

}


QColor Fort14_new::GetSolidBoundaryColor()
{

}


QColor Fort14_new::GetSolidFillColor()
{

}


QColor Fort14_new::GetSolidOutlineColor()
{

}


void Fort14_new::SetGradientFillColors(QGradientStops newStops)
{

}


void Fort14_new::SetGradientOutlineColors(QGradientStops newStops)
{

}


void Fort14_new::SetSolidBoundaryColor(QColor newColor)
{

}


void Fort14_new::SetSolidFillColor(QColor newColor)
{

}


void Fort14_new::SetSolidOutlineColor(QColor newColor)
{

}


void Fort14_new::SelectCircle(int x, int y, int radius)
{

}


void Fort14_new::SelectPoint(int x, int y)
{

}


void Fort14_new::SelectPolygon(std::vector<Point> polyLine)
{

}


void Fort14_new::SelectRectangle(int l, int r, int b, int t)
{

}
