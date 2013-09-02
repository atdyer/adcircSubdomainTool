#include "GradientSliderWidget.h"

GradientSliderWidget::GradientSliderWidget(QWidget *parent) :
	QWidget(parent),
	gradientFrame(0)
{
	sliderWidth = 0;
	sliderHeight = 0;

	bottomSliderClicked = false;

	bottomSlider = 0;
	topSlider = 0;

	CreateLayout();
}


void GradientSliderWidget::paintEvent(QPaintEvent *)
{

}


void GradientSliderWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (bottomSliderClicked)
	{
		std::cout << "Bottom slider clicked" << std::endl;
		if (event->y() > contentsRect().y() && event->y() < contentsRect().y()+contentsRect().height())
		{
			bottomSlider->move(bottomSlider->x(), event->y());
		} else {
			std::cout << "Nope" << std::endl;
		}
	}
}


void GradientSliderWidget::resizeEvent(QResizeEvent *event)
{
	int width = contentsRect().width();
	int height = contentsRect().height();
	if (gradientFrame)
	{
		gradientFrame->resize(width-sliderWidth, height-sliderHeight);
		if (sliderHeight%2 == 1)
		{
			gradientFrame->move(0, (sliderHeight-1)/2);
		} else {
			gradientFrame->move(0, sliderHeight/2);
		}
	}

	if (bottomSlider)
	{
		bottomSlider->move(width-sliderWidth, height-sliderHeight);
	}
	if (topSlider)
	{
		topSlider->move(width-sliderWidth, 0);
	}

//	TriangleSliderButton *currentSlider = 0;
//	for (std::vector<TriangleSliderButton*>::iterator it = sliders.begin(); it != sliders.end(); ++it)
//	{
//		currentSlider = *it;
//		if (currentSlider)
//		{
//			currentSlider->move(width-sliderWidth, 50);
//		}
//	}
}


void GradientSliderWidget::CreateLayout()
{
	gradientFrame = new GradientSliderFrame(this);

	bottomSlider = new TriangleSliderButton(this);
	topSlider = new TriangleSliderButton(this);

	connect(bottomSlider, SIGNAL(pressed()), this, SLOT(BottomSliderClicked()));
	connect(bottomSlider, SIGNAL(released()), this, SLOT(BottomSliderClicked()));

	sliderWidth = bottomSlider->width();
	sliderHeight = bottomSlider->height();

	sliders.push_back(bottomSlider);
	sliders.push_back(topSlider);
}


void GradientSliderWidget::BottomSliderClicked()
{
	std::cout << "Pressed" << std::endl;
	bottomSliderClicked = !bottomSliderClicked;
}
