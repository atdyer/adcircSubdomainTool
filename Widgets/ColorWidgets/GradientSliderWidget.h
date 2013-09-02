#ifndef GRADIENTSLIDERWIDGET_H
#define GRADIENTSLIDERWIDGET_H

#include <QWidget>
#include <QBoxLayout>
#include <QPaintEvent>
#include <QResizeEvent>

#include <iostream>

#include "Widgets/ColorWidgets/GradientSliderFrame.h"
#include "Widgets/ColorWidgets/TriangleSliderButton.h"

class GradientSliderWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit GradientSliderWidget(QWidget *parent = 0);

	protected:

		void	paintEvent(QPaintEvent *);
		void	mouseMoveEvent(QMouseEvent *event);
		void	resizeEvent(QResizeEvent *event);

	private:

		GradientSliderFrame*			gradientFrame;
		std::vector<TriangleSliderButton*>	sliders;

		int	sliderWidth;
		int	sliderHeight;

		bool	bottomSliderClicked;

		TriangleSliderButton *bottomSlider;
		TriangleSliderButton *topSlider;

		void	CreateLayout();
		
	signals:
		
	public slots:

		void	BottomSliderClicked();
		
};

#endif // GRADIENTSLIDERWIDGET_H
