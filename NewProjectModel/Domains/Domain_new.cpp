#include "Domain_new.h"

Domain_new::Domain_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	camera(0),
	fort14(0),
	progressBar(0),
	projectFile(projectFile),
	selectionLayer(0),
	currentMode(DisplayAction),
	oldx(0.0),
	oldy(0.0),
	newx(0.0),
	newy(0.0),
	dx(0.0),
	dy(0.0),
	pushedButton(Qt::NoButton),
	clicking(false),
	mouseMoved(false)
{
	camera = new GLCamera();
}


Domain_new::~Domain_new()
{
	delete camera;
}


void Domain_new::Draw()
{
	if (fort14)
		fort14->Draw();

	if (selectionLayer)
		selectionLayer->Draw();
}


void Domain_new::MouseClick(QMouseEvent *event)
{
	clicking = true;
	pushedButton = event->button();

	oldx = event->x();
	oldy = event->y();
	mouseMoved = false;

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->MouseClick(event);

	emit updateGL();
}


void Domain_new::MouseMove(QMouseEvent *event)
{
	mouseMoved = true;
	newx = event->x();
	newy = event->y();
	dx = newx-oldx;
	dy = newy-oldy;

	CalculateMouseCoordinates();

	if (currentMode == DisplayAction)
	{
		if (clicking)
		{
			Pan(dx, dy);
			emit updateGL();
		}
	}
	else if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseMove(event);
		emit updateGL();
	}

	oldx = newx;
	oldy = newy;
}


void Domain_new::MouseRelease(QMouseEvent *event)
{
	clicking = false;
	mouseMoved = false;

	oldx = event->x();
	oldy = event->y();

	if (currentMode == SelectionAction && selectionLayer)
	{
		selectionLayer->MouseRelease(event);
	}

	emit updateGL();
}


void Domain_new::MouseWheel(QWheelEvent *event)
{
	if (!clicking)
		Zoom(event->delta());

	CalculateMouseCoordinates();

	emit updateGL();
}


void Domain_new::KeyPress(QKeyEvent *event)
{
//	if (event->key() == Qt::Key_O)
//		ToggleTerrainQuadtree();
//	else if (event->key() == Qt::Key_E)
//		UseTool(ClickToolType, ElementSelection);

	if (currentMode == SelectionAction && selectionLayer)
		selectionLayer->KeyPress(event);
}


void Domain_new::Redo()
{
	if (selectionLayer)
		selectionLayer->Redo();
	emit updateGL();
}


bool Domain_new::RedoAvailable()
{
	if (selectionLayer)
		return selectionLayer->GetRedoAvailable();
	return false;
}


void Domain_new::SetCamera(GLCamera *newCam)
{
	camera = newCam;
	if (fort14)
		fort14->SetCamera(camera);

}


void Domain_new::SetProgressBar(QProgressBar* newBar)
{
	progressBar = newBar;
}


void Domain_new::SetWindowSize(float w, float h)
{
	if (camera)
		camera->SetWindowSize(-1.0*w/h, 1.0*w/h, -1.0, 1.0, -1000.0, 1000.0);

	if (selectionLayer)
		selectionLayer->WindowSizeChanged(w, h);
}


void Domain_new::Undo()
{
	if (selectionLayer)
		selectionLayer->Undo();
	emit updateGL();
}


bool Domain_new::UndoAvailable()
{
	if (selectionLayer)
		return selectionLayer->GetUndoAvailable();
	return false;
}


void Domain_new::UseTool(ToolType tool, SelectionType selection)
{
	currentMode = SelectionAction;
	if (selectionLayer)
		selectionLayer->UseTool(tool, selection);
	emit setCursor(Qt::CrossCursor);
}


void Domain_new::Zoom(float zoomAmount)
{
	if (camera)
		camera->Zoom(zoomAmount);
	emit updateGL();
}


void Domain_new::Pan(float dx, float dy)
{
	if (camera)
		camera->Pan(dx, dy);
	emit updateGL();
}


void Domain_new::CalculateMouseCoordinates()
{
	if (camera)
	{
		float glX, glY, domX, domY;
		camera->GetUnprojectedPoint(newx, newy, &glX, &glY);
		if (fort14)
		{
			domX = fort14->GetUnprojectedX(glX);
			domY = fort14->GetUnprojectedY(glY);
			emit mouseX(domX);
			emit mouseY(domY);
		} else {
			emit mouseX(glX);
			emit mouseY(glY);
		}
	} else {
		emit mouseX(newx);
		emit mouseY(newy);
	}
}


void Domain_new::SetTerrainSolidOutline(QColor newColor)
{
	if (fort14)
		fort14->SetSolidOutlineColor(newColor);

	emit updateGL();
}


void Domain_new::SetTerrainSolidFill(QColor newColor)
{
	if (fort14)
		fort14->SetSolidFillColor(newColor);

	emit updateGL();
}


void Domain_new::SetTerrainGradientOutline(QGradientStops newStops)
{
	if (fort14)
		fort14->SetGradientOutlineColors(newStops);

	emit updateGL();
}


void Domain_new::SetTerrainGradientFill(QGradientStops newStops)
{
	if (fort14)
		fort14->SetGradientFillColors(newStops);

	emit updateGL();
}


void Domain_new::EnterDisplayMode()
{
	currentMode = DisplayAction;
	emit setCursor(Qt::ArrowCursor);
}

