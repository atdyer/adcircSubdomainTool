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

		unsigned int	AddSlider();
		void		RemoveSlider(unsigned int sliderID);

		void	SetMinValue(float newMin);
		void	SetMaxValue(float newMax);

		void	SetSliderColor(unsigned int sliderID, QColor newColor);
		void	SetSliderValue(unsigned int sliderID, float newValue);

		float	GetMinValue();
		float	GetMaxValue();

		QColor	GetSliderColor(unsigned int sliderID);
		float	GetSliderValue(unsigned int sliderID);

	protected:

		void	mouseMoveEvent(QMouseEvent *event);
		void	resizeEvent(QResizeEvent *event);

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

		void	CreateLayout();
		void	PositionSliders();
		void	PositionGradientFrame();
		void	UpdateGradientStops();
		void	CheckSliderCount();

		int	MapValueToY(float val);
		float	MapYToValue(int y);
		bool	YValueInRange(int y);
		
	signals:

		void	sliderAdded(unsigned int, float, QColor);
		void	currentSliderChanged(unsigned int, float, QColor);
		void	sliderColorChanged(unsigned int, QColor);
		void	sliderValueChanged(unsigned int, float);
		
	public slots:

		void	sliderPressed(unsigned int sliderID);
		void	sliderReleased(unsigned int sliderID);
};

#endif // GRADIENTSLIDERWIDGET_H