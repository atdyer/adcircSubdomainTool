#ifndef TRIANGLESLIDERBUTTON_H
#define TRIANGLESLIDERBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>

class TriangleSliderButton : public QPushButton
{
		Q_OBJECT
	public:
		explicit TriangleSliderButton(QWidget *parent = 0);

		void	SetColor(QColor newColor);
		QColor	GetColor();

	protected:

		int	triangleWidth;
		int	triangleHeight;

		void	BuildTriangle();
		void	paintEvent(QPaintEvent *event);
		void	resizeEvent(QResizeEvent *event);

	private:

		QColor		triangleColor;
		QPolygon	triangle;
		
	signals:
		
	public slots:
		
};

#endif // TRIANGLESLIDERBUTTON_H
