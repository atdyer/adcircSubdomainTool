#include "ColorGradientFrame.h"

ColorGradientFrame::ColorGradientFrame(QWidget *parent) : QFrame(parent)
{
	currentHue = 0;
	currentSaturation = 0;
	currentValue = 200;
	currentAlpha = 255;

	minHue = 0;
	maxHue = 359;
	minSat = 0;
	maxSat = 255;
	minVal = 0;
	maxVal = 255;
	minAlp = 0;
	maxAlp = 255;

	BuildPixMap();

}


void ColorGradientFrame::paintEvent(QPaintEvent *)
{
	QPainter painter(this);
	drawFrame(&painter);
	QRect rectangle = contentsRect();
	QStyleOptionFrame optionFrame;
	optionFrame.initFrom(this);
	if (optionFrame.state & QStyle::State_Enabled)
	{
		painter.drawPixmap(rectangle.topLeft(), pixMap);
		DrawCrosshair(&painter, crosshairPoint);
	} else {
		QIcon icon(pixMap);
		icon.paint(&painter, rectangle, 0, QIcon::Disabled);
	}
}


void ColorGradientFrame::resizeEvent(QResizeEvent *event)
{
	QFrame::resizeEvent(event);
	BuildPixMap();
	update();
}


void ColorGradientFrame::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		QPoint point = event->pos();
		int hue = HueFromX(point.x() - contentsRect().x());
		int sat = SatFromY(point.y() - contentsRect().y());
		if (hue < minHue || hue > maxHue || sat < minSat || sat > maxSat)
		{
			return;
		}
		emit colorPicked(QColor::fromHsv(hue, sat, currentValue));
		crosshairPoint = point;
		update();
	} else {
		QFrame::mouseMoveEvent(event);
	}
}


void ColorGradientFrame::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::LeftButton)
	{
		QPoint point = event->pos();
		if (!contentsRect().contains(point))
		{
			return;
		}
		int hue = HueFromX(point.x() - contentsRect().x());
		int sat = SatFromY(point.y() - contentsRect().y());
		if (hue < minHue || hue > maxHue || sat < minSat || sat > maxSat)
		{
			return;
		}
		emit colorPicked(QColor::fromHsv(hue, sat, currentValue));
		crosshairPoint = point;
		update();
	} else {
		QFrame::mouseMoveEvent(event);
	}
}


void ColorGradientFrame::BuildPixMap()
{
	int height = contentsRect().height();
	int width = contentsRect().width();
	QImage image (width, height, QImage::Format_RGB32);
	for (int y=0; y<height; ++y)
	{
		for (int x=0; x<width; ++x)
		{
			QColor c;
			c.setHsv(HueFromX(x), SatFromY(y), currentValue);
			image.setPixel(x, y, c.rgb());
		}
	}
	pixMap = QPixmap::fromImage(image);
}


void ColorGradientFrame::DrawCrosshair(QPainter *p, const QPoint &pt)
{
	p->save();
	p->setPen(Qt::black);
	p->drawLine(pt-QPoint(0, -3), pt-QPoint(0, -1));
	p->drawLine(pt-QPoint(0, 1), pt-QPoint(0, 3));
	p->drawLine(pt-QPoint(-3, 0), pt-QPoint(-1, 0));
	p->drawLine(pt-QPoint(1, 0), pt-QPoint(3, 0));
	p->restore();
}


int ColorGradientFrame::HueFromX(int x)
{
	return maxHue - x * (maxHue - minHue) / contentsRect().width();
}


int ColorGradientFrame::SatFromY(int y)
{
	return maxSat - y * (maxSat - minSat) / contentsRect().height();
}
