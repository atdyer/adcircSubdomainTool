#ifndef SELECTIONTOOL_H
#define SELECTIONTOOL_H

#include <QObject>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include <QString>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"

class SelectionTool : public QObject
{
		Q_OBJECT
	public:
		SelectionTool();

		virtual void	Draw() = 0;
		virtual void	SetCamera(GLCamera *cam) = 0;
		virtual void	SetTerrainLayer(TerrainLayer *layer) = 0;
		virtual void	SetViewportSize(float w, float h) = 0;
		virtual void	SetSelectionMode(SelectionType newMode) = 0;

		virtual void	MouseClick(QMouseEvent *event) = 0;
		virtual void	MouseMove(QMouseEvent *event) = 0;
		virtual void	MouseRelease(QMouseEvent *event) = 0;
		virtual void	MouseWheel(QWheelEvent *event) = 0;
		virtual void	KeyPress(QKeyEvent *event) = 0;

		virtual void	UseTool() = 0;
		void		UseTool(SelectionType newMode);

		virtual std::vector<Element*>	GetSelectedElements() = 0;

	signals:

		void	Message(QString);
		void	Instructions(QString);
		void	ToolFinishedDrawing();
};

#endif // SELECTIONTOOL_H
