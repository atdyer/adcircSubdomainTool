#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QToolButton>

class ColorButton : public QToolButton
{
		Q_OBJECT
	public:
		explicit ColorButton(QWidget *parent = 0);

		void	SetBackgroundColor(QColor color);

	private:

		QColor	currentColor;

	public slots:

		void	click();

	signals:

		void	clicked(QColor);
};

#endif // COLORBUTTON_H
