#ifndef GRADIENTSLIDERWIDGET_H
#define GRADIENTSLIDERWIDGET_H

#include <QWidget>
#include <QMap>
#include <QLabel>
#include <QBoxLayout>

#include <iostream>

#include "Widgets/ColorWidgets/GradientSliderFrame.h"
#include "Widgets/ColorWidgets/TriangleSliderButton.h"

class GradientSliderWidget : public QWidget
{
		Q_OBJECT
	public:
		explicit GradientSliderWidget(QWidget *parent = 0);

	protected:

		void	mouseMoveEvent(QMouseEvent *event);
		void	resizeEvent(QResizeEvent *event);

		void	SetMinValue(float newMin);
		void	SetMaxValue(float newMax);

		void	SetSliderColor(unsigned int sliderID, QColor newColor);
		void	SetSliderValue(unsigned int sliderID, float newValue);

		QColor	GetSliderColor(unsigned int sliderID);
		float	GetSliderValue(unsigned int sliderID);

	private:

		QBoxLayout*	layout;

		QLabel*	minLabel;
		QLabel*	maxLabel;

		GradientSliderFrame*				gradientFrame;
		QMap<unsigned int, TriangleSliderButton*>	sliders;

		int		sliderWidth;
		int		sliderHeight;
		int		sliderTop;
		int		sliderBottom;
		int		sliderX;
		unsigned int	pressedSlider;

		float	minValue;
		float	maxValue;

		void		CreateLayout();
		unsigned int	AddSlider();
		void		RemoveSlider(unsigned int sliderID);
		void		PositionSliders();
		void		UpdateGradientStops();

		int	MapValueToY(float val);
		float	MapYToValue(int y);
		bool	YValueInRange(int y);
		
	signals:
		
	public slots:

		void	sliderPressed(unsigned int sliderID);
		void	sliderReleased(unsigned int sliderID);
};

#endif // GRADIENTSLIDERWIDGET_H
