#include "TriangleSliderButton.h"

unsigned int	TriangleSliderButton::nextID = 1;
int		TriangleSliderButton::sliderWidth = 13;
int		TriangleSliderButton::sliderHeight = 13;

TriangleSliderButton::TriangleSliderButton(QWidget *parent) :
	QPushButton(parent),
	sliderID(nextID),
	triangleWidth(sliderWidth),
	triangleHeight(sliderHeight),
	currentValue(0.0)
{
	++nextID;

	triangleColor = QColor::fromRgb(0, 0, 0);

	isRemovable = true;

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
	setFixedSize(triangleWidth, triangleHeight);
}


unsigned int TriangleSliderButton::GetID()
{
	return sliderID;
}


void TriangleSliderButton::SetRemovable(bool newRemovable)
{
	isRemovable = newRemovable;
}


void TriangleSliderButton::SetColor(QColor newColor)
{
	triangleColor = newColor;
	update();
}


void TriangleSliderButton::SetValue(float newValue)
{
	currentValue = newValue;
}


QColor TriangleSliderButton::GetColor()
{
	return triangleColor;
}


float TriangleSliderButton::GetValue()
{
	return currentValue;
}


void TriangleSliderButton::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);

	QPainterPath path;
	path.setFillRule(Qt::WindingFill);
	path.addPolygon(triangle);
	painter.drawPath(path);
	painter.fillPath(path, triangleColor);
}


void TriangleSliderButton::resizeEvent(QResizeEvent *event)
{
	QPushButton::resizeEvent(event);
	BuildTriangle();
}


void TriangleSliderButton::mousePressEvent(QMouseEvent *)
{
	emit sliderPressed(sliderID);
}


void TriangleSliderButton::mouseReleaseEvent(QMouseEvent *)
{
	emit sliderReleased(sliderID);
}


void TriangleSliderButton::contextMenuEvent(QContextMenuEvent *event)
{
	if (isRemovable)
	{
		QMenu contextMenu;
		contextMenu.addAction("Remove Color");
		QPoint location = mapToGlobal(event->pos());

		QAction *selectedItem = contextMenu.exec(location);
		if (selectedItem)
			emit removeSlider();
	}
}


void TriangleSliderButton::BuildTriangle()
{
	int width = contentsRect().width();
	int height = contentsRect().height();

	triangle.clear();

	triangle << QPoint(width-1, 0);
	if (height%2 == 1)
	{
		triangle << QPoint(0, (width-1)/2);
	} else {
		triangle << QPoint(0, width/2);
	}
	triangle << QPoint(width-1, height-1);
	triangle << QPoint(width-1, 0);
}

