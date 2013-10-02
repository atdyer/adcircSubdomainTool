#ifndef FORT14_new_H
#define FORT14_new_H

#include <QObject>
#include <QProgressBar>
#include <QThread>

#include "NewProjectModel/Files/ProjectFile_new.h"
#include "NewProjectModel/Files/Workers/Fort14Reader.h"

#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/GradientShader.h"
#include "OpenGL/Shaders/SolidShader.h"

#include "Quadtree/Quadtree.h"

class Fort14_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14_new(QObject *parent=0);
		Fort14_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);
		~Fort14_new();

		void	Draw();

		std::vector<Element>*	GetElements();
		QString			GetFilePath();
		ShaderType		GetFillShaderType();
		QGradientStops		GetGradientBoundaryColors();
		QGradientStops		GetGradientFillColors();
		QGradientStops		GetGradientOutlineColors();
		float			GetMaxX();
		float			GetMaxY();
		float			GetMaxZ();
		float			GetMinX();
		float			GetMinY();
		float			GetMinZ();
		ShaderType		GetOutlineShaderType();
		float			GetUnprojectedX(float x);
		float			GetUnprojectedY(float y);

		std::vector<Element*>	GetSelectedElements();
		QColor			GetSolidBoundaryColor();
		QColor			GetSolidFillColor();
		QColor			GetSolidOutlineColor();

		void			SetGradientFillColors(QGradientStops newStops);
		void			SetGradientOutlineColors(QGradientStops newStops);
		void			SetProgressBar(QProgressBar *newBar);
		void			SetSolidBoundaryColor(QColor newColor);
		void			SetSolidFillColor(QColor newColor);
		void			SetSolidOutlineColor(QColor newColor);

	private:

		QString			domainName;
		std::vector<Element>	elements;
		float			maxX;
		float			maxY;
		float			maxZ;
		float			minX;
		float			minY;
		float			minZ;
		std::vector<Node>	nodes;
		QProgressBar*		progressBar;
		ProjectFile_new*	projectFile;
		Quadtree*		quadtree;
		bool			readingLock;

		void	ReadFile();

	public slots:

		void	SelectCircle(int x, int y, int radius);
		void	SelectPoint(int x, int y);
		void	SelectPolygon(std::vector<Point> polyLine);
		void	SelectRectangle(int l, int r, int b, int t);

	protected slots:

		void	LockFile();
		void	Progress(int percent);
		void	SetDomainBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ);
		void	UnlockFile();


};

#endif // FORT14_new_H
