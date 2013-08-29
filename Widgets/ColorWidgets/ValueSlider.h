#ifndef VALUESLIDER_H
#define VALUESLIDER_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QMouseEvent>

#include <iostream>

class ValueSlider : public QFrame
{
		Q_OBJECT
	public:
		ValueSlider(QWidget *parent = 0);

		int	GetSelectedValue();

	protected:

		void	paintEvent(QPaintEvent *);
		void	resizeEvent(QResizeEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);

	private:

		QPoint	trianglePoint;
		QPixmap	pixMap;

		int	currentHue;
		int	currentSaturation;
		int	currentValue;

		int	minHue, maxHue;
		int	minSat, maxSat;
		int	minVal, maxVal;
		int	triangleWidth;
		int	triangleHeight;

		void	BuildPixMap();
		void	DrawTriangle(QPainter *p, const QPoint &pt);
		int	ValFromY(int y);
		int	ValToY(int val);

	signals:

		void	colorPicked(QColor);

	public slots:

		void	setColor(const QColor &c);
};

#endif // VALUESLIDER_H
