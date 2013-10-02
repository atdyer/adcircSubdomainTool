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
	progressBar(0),
	projectFile(0),
	quadtree(0),
	readingLock(false)
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
	progressBar(0),
	projectFile(projectFile),
	quadtree(0),
	readingLock(false)
{
	ReadFile();
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
	progressBar(0),
	projectFile(projectFile),
	quadtree(0),
	readingLock(false)
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


QString Fort14_new::GetFilePath()
{
	if (projectFile)
	{
		if (domainName.isEmpty())
		{
			return projectFile->GetFullDomainFort14();
		}
		else
		{
			return projectFile->GetSubDomainFort14(domainName);
		}
	}
	return QString();
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


std::vector<Element*> Fort14_new::GetSelectedElements()
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


void Fort14_new::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;
//	ReadFile();
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


void Fort14_new::ReadFile()
{
	if (!readingLock)
	{
		std::cout << "Launching from thread: " << this->thread() << std::endl;
		QString filePath = GetFilePath();
		if (QFile(filePath).exists())
		{
			QThread *thread = new QThread();
			Fort14Reader *worker = new Fort14Reader(filePath, &nodes, &elements);

			worker->moveToThread(thread);
			connect(thread, SIGNAL(started()), worker, SLOT(ReadFile()));
			connect(worker, SIGNAL(Progress(int)), this, SLOT(Progress(int)));
			connect(worker, SIGNAL(FinishedReading()), this, SLOT(UnlockFile()));
			connect(worker, SIGNAL(FinishedReading()), thread, SLOT(quit()));
			connect(worker, SIGNAL(FinishedReading()), worker, SLOT(deleteLater()));
			connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

			if (progressBar)
			{
				connect(worker, SIGNAL(StartedReading()), progressBar, SLOT(show()));
				connect(worker, SIGNAL(FinishedReading()), progressBar, SLOT(hide()));
			}

			LockFile();
			thread->start();
		}
	}
}


void Fort14_new::Progress(int percent)
{
	if (progressBar)
	{
		progressBar->setValue(percent);
		if (percent == 100)
		{
			progressBar->reset();
		}
	}
}


void Fort14_new::LockFile()
{
	std::cout << "\n-----\nLocking file.\nNode Count: " << nodes.size() << "\nElement Count: " << elements.size() <<
		     "\n-----" << std::endl;
	readingLock = true;
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


void Fort14_new::UnlockFile()
{
	std::cout << "\n-----\nUnlocking file.\nNode Count: " << nodes.size() << "\nElement Count: " << elements.size() <<
		     "\n-----" << std::endl;
	readingLock = false;
}
