#ifndef COLORGRADIENTFRAME_H
#define COLORGRADIENTFRAME_H

#include <QWidget>
#include <QFrame>
#include <QPixmap>
#include <QPainter>
#include <QStyleOptionFrame>
#include <QMouseEvent>

class ColorGradientFrame : public QFrame
{
		Q_OBJECT
	public:
		ColorGradientFrame(QWidget *parent = 0);

		QColor	GetSelectedColor();

	protected:

		void	paintEvent(QPaintEvent *);
		void	resizeEvent(QResizeEvent *event);
		void	mousePressEvent(QMouseEvent *event);
		void	mouseMoveEvent(QMouseEvent *event);

	private:

		QPoint	crosshairPoint;
		QPixmap	pixMap;

		int	currentHue;
		int	currentSaturation;
		int	currentValue;

		int	minHue, maxHue;
		int	minSat, maxSat;
		int	minVal, maxVal;

		void	BuildPixMap();
		void	DrawCrosshair(QPainter *p, const QPoint &pt);
		int	HueFromX(int x);
		int	HueToX(int hue);
		int	SatFromY(int y);
		int	SatToY(int sat);

	signals:

		void	colorPicked(QColor);

	public slots:

		void	setColor(const QColor &c);
};

#endif // COLORGRADIENTFRAME_H
