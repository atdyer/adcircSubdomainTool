#ifndef FORT14_new_H
#define FORT14_new_H

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/GradientShader.h"
#include "OpenGL/Shaders/SolidShader.h"

class Fort14_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort14_new(QObject *parent=0);
		Fort14_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);
		~Fort14_new();

		void	Draw();

		ShaderType	GetFillShaderType();
		QGradientStops	GetGradientBoundaryColors();
		QGradientStops	GetGradientFillColors();
		QGradientStops	GetGradientOutlineColors();
		ShaderType	GetOutlineShaderType();
		QColor		GetSolidBoundaryColor();
		QColor		GetSolidFillColor();
		QColor		GetSolidOutlineColor();

		void		SetGradientFillColors(QGradientStops newStops);
		void		SetGradientOutlineColors(QGradientStops newStops);
		void		SetSolidBoundaryColor(QColor newColor);
		void		SetSolidFillColor(QColor newColor);
		void		SetSolidOutlineColor(QColor newColor);

	private:

		QString			domainName;
		ProjectFile_new*	projectFile;
};

#endif // FORT14_new_H
