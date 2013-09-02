#ifndef TRIANGLESLIDERBUTTON_H
#define TRIANGLESLIDERBUTTON_H

#include <QPushButton>
#include <QWidget>
#include <QPainter>
#include <QResizeEvent>
#include <QPaintEvent>
#include <QMenu>
#include <QContextMenuEvent>

class TriangleSliderButton : public QPushButton
{
		Q_OBJECT
	public:
		explicit TriangleSliderButton(QWidget *parent = 0);

		void	SetRemovable(bool newRemovable);
		void	SetColor(QColor newColor);
		QColor	GetColor();

	protected:

		void	paintEvent(QPaintEvent *event);
		void	resizeEvent(QResizeEvent *event);
		void	contextMenuEvent(QContextMenuEvent *event);

	private:

		QColor		triangleColor;
		QPolygon	triangle;
		int		triangleWidth;
		int		triangleHeight;
		bool		isRemovable;

		void		BuildTriangle();

	signals:

		void	removeSlider();
};

#endif // TRIANGLESLIDERBUTTON_H
