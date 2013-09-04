#include "GradientSliderWidget.h"

GradientSliderWidget::GradientSliderWidget(QWidget *parent) :
	QWidget(parent),
	minLabel(0),
	maxLabel(0),
	gradientFrame(0)
{
	sliderWidth = TriangleSliderButton::sliderWidth;
	sliderHeight = TriangleSliderButton::sliderHeight;

	sliderTop = 0;
	sliderBottom = 0;
	sliderX = 0;
	pressedSlider = 0;

	minValue = 0.0;
	maxValue = 1.0;

	CreateLayout();
}


unsigned int GradientSliderWidget::AddSlider()
{
	TriangleSliderButton *newSlider = new TriangleSliderButton(this);
	unsigned int newID = newSlider->GetID();
	if (!sliders.contains(newID))
	{
		newSlider->SetValue((minValue + maxValue) / 2.0);
		newSlider->SetColor(QColor::fromRgb(0, 0, 0));
		connect(newSlider, SIGNAL(sliderPressed(uint)), this, SLOT(sliderPressed(uint)));
		connect(newSlider, SIGNAL(sliderReleased(uint)), this, SLOT(sliderReleased(uint)));
		sliders[newID] = newSlider;

		newSlider->show();
		CheckSliderCount();
		PositionGradientFrame();
		PositionSliders();

		emit sliderAdded(newID, newSlider->GetValue(), newSlider->GetColor());

		return newID;
	} else {
		delete newSlider;
	}

	return 0;
}


void GradientSliderWidget::RemoveSlider(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		TriangleSliderButton *currentSlider = sliders.take(sliderID);
		delete currentSlider;
	}

	CheckSliderCount();
	PositionSliders();
}


void GradientSliderWidget::SetMinValue(float newMin)
{
	minValue = newMin;
	if (minLabel)
		minLabel->setText(QString("Minimum: ").append(QString::number(minValue, 'f', 4)));
	PositionSliders();
	UpdateGradientStops();
}


void GradientSliderWidget::SetMaxValue(float newMax)
{
	maxValue = newMax;
	if (maxLabel)
		maxLabel->setText(QString("Maximum: ").append(QString::number(maxValue, 'f', 4)));
	PositionSliders();
	UpdateGradientStops();
}


void GradientSliderWidget::SetSliderColor(unsigned int sliderID, QColor newColor)
{
	if (sliders.contains(sliderID))
	{
		sliders.value(sliderID)->SetColor(newColor);
		emit sliderColorChanged(sliderID, newColor);
		UpdateGradientStops();
		update();
	}
}


void GradientSliderWidget::SetSliderValue(unsigned int sliderID, float newValue)
{
	if (sliders.contains(sliderID))
	{
		sliders.value(sliderID)->SetValue(newValue);
		emit sliderValueChanged(sliderID, newValue);
		UpdateGradientStops();
	}
}


float GradientSliderWidget::GetMinValue()
{
	return minValue;
}


float GradientSliderWidget::GetMaxValue()
{
	return maxValue;
}


QColor GradientSliderWidget::GetSliderColor(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		return sliders.value(sliderID)->GetColor();
	}
	return QColor::fromRgb(0, 0, 0);
}


float GradientSliderWidget::GetSliderValue(unsigned int sliderID)
{
	if (sliders.contains(sliderID))
	{
		return sliders.value(sliderID)->GetValue();
	}
	return 0.0;
}


void GradientSliderWidget::mouseMoveEvent(QMouseEvent *event)
{
	if (pressedSlider != 0 && sliders.contains(pressedSlider))
	{
		int yLoc = event->y();
		if (YValueInRange(yLoc))
		{
			TriangleSliderButton *currentSlider = sliders.value(pressedSlider, 0);
			if (currentSlider)
			{
				currentSlider->move(sliderX, yLoc-sliderHeight/2);
				float newValue = MapYToValue(yLoc);
				SetSliderValue(pressedSlider, newValue);
				emit sliderValueChanged(pressedSlider, newValue);
				UpdateGradientStops();
				update();
			}
		}
	}
}


void GradientSliderWidget::resizeEvent(QResizeEvent *)
{
	PositionGradientFrame();
	PositionSliders();
}


void GradientSliderWidget::CreateLayout()
{
	layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

	gradientFrame = new GradientSliderFrame(this);

	maxLabel = new QLabel(QString("Maximum: ").append(QString::number(maxValue, 'f', 4)), this);
	minLabel = new QLabel(QString("Minimum: ").append(QString::number(minValue, 'f', 4)), this);
	maxLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	minLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
	maxLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
	minLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

	layout->addWidget(maxLabel);
	layout->addWidget(gradientFrame);
	layout->addWidget(minLabel);

//	unsigned int initialBottomSlider = AddSlider();
//	unsigned int initialTopSlider = AddSlider();

//	if (initialBottomSlider)
//	{
//		SetSliderColor(initialBottomSlider, QColor::fromRgb(0, 255, 0));
//		SetSliderValue(initialBottomSlider, minValue);
//	}

//	if (initialTopSlider)
//	{
//		SetSliderColor(initialTopSlider, QColor::fromRgb(0, 0, 255));
//		SetSliderValue(initialTopSlider, maxValue);
//	}
}


void GradientSliderWidget::PositionSliders()
{
	TriangleSliderButton* currentSlider = 0;
	QMap<unsigned int, TriangleSliderButton*>::const_iterator it = sliders.constBegin();
	while (it != sliders.constEnd())
	{
		currentSlider = it.value();
		if (currentSlider)
		{
			float currentValue = currentSlider->GetValue();
			currentSlider->move(sliderX, MapValueToY(currentValue));
		}
		++it;
	}

	UpdateGradientStops();
}


void GradientSliderWidget::PositionGradientFrame()
{
	if (gradientFrame)
	{
		gradientFrame->resize(gradientFrame->width()-sliderWidth, gradientFrame->height());
		sliderTop = gradientFrame->y();
		sliderBottom = gradientFrame->y() + gradientFrame->height();
		sliderX = gradientFrame->x() + gradientFrame->width();
	}
}


void GradientSliderWidget::UpdateGradientStops()
{
	if (gradientFrame)
	{
		QGradientStops stops;

		foreach(TriangleSliderButton* slider, sliders)
		{
			float valuePercentage = (slider->GetValue() - minValue) / (maxValue - minValue);
			stops << QGradientStop(1.0-valuePercentage, slider->GetColor());
		}

		gradientFrame->SetStops(stops);
	}
}


void GradientSliderWidget::CheckSliderCount()
{
	TriangleSliderButton *currentSlider = 0;
	bool removable = sliders.size() <= 2 ? false : true;

	QMap<unsigned int, TriangleSliderButton*>::const_iterator it = sliders.constBegin();
	while (it != sliders.constEnd())
	{
		currentSlider = *it;
		if (currentSlider)
		{
			currentSlider->SetRemovable(removable);
		}
		++it;
	}
}


int GradientSliderWidget::MapValueToY(float val)
{
	float valuePercentage = (val - minValue) / (maxValue - minValue);
	float remainderPercentage = 1.0 - valuePercentage;
	return (sliderTop + (int)(remainderPercentage*(sliderBottom - sliderTop))) - sliderHeight/2;
}


float GradientSliderWidget::MapYToValue(int y)
{
	float yPercentage = float(sliderBottom - y) / float(sliderBottom - sliderTop);
	float mappedValue = minValue + yPercentage*(maxValue - minValue);
	if (mappedValue < minValue)
		return minValue;
	if (mappedValue > maxValue)
		return maxValue;
	return mappedValue;
}


bool GradientSliderWidget::YValueInRange(int y)
{
	return (y > sliderTop && y < sliderBottom);
}


void GradientSliderWidget::sliderPressed(unsigned int sliderID)
{
	pressedSlider = sliderID;
	emit currentSliderChanged(sliderID, GetSliderValue(sliderID), GetSliderColor(sliderID));
}


void GradientSliderWidget::sliderReleased(unsigned int sliderID)
{
	if (pressedSlider == sliderID)
	{
		pressedSlider = 0;
	}
}

