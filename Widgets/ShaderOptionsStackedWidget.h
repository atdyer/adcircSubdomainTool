#ifndef SHADEROPTIONSSTACKEDWIDGET_H
#define SHADEROPTIONSSTACKEDWIDGET_H

#include <QStackedWidget>
#include <QColorDialog>
#include <QMap>
#include <QListWidget>
#include <Widgets/ColorWidgets/ColorGradientFrame.h>
#include <Widgets/ColorWidgets/ValueSlider.h>
#include <Widgets/ColorWidgets/SliderItemDelegate.h>

namespace Ui {
	class ShaderOptionsStackedWidget;
}

class ShaderOptionsStackedWidget : public QStackedWidget
{
		Q_OBJECT
		
	public:
		explicit ShaderOptionsStackedWidget(QWidget *parent = 0);
		~ShaderOptionsStackedWidget();
		
	private:
		Ui::ShaderOptionsStackedWidget *ui;

		QMap<unsigned int, QListWidgetItem*>	sliderListItems;

		SliderItemDelegate *delegate;

		void	SetupGradientPicker();


	protected slots:

		void	colorChanged(const QColor &c);
		void	addCurrentColorToCustomColors();

		/* Slider slots */
		void	addSlider();
		void	sliderAdded(unsigned int sliderID, float sliderValue, QColor sliderColor);
		void	sliderValueSet(int row, float newValue);
		void	removeSlider(unsigned int sliderID);
		void	currentSliderChanged(unsigned int sliderID, float sliderValue, QColor sliderColor);
		void	gradientSliderColorChanged(unsigned int sliderID, QColor newColor);
		void	gradientSliderValueChanged(unsigned int sliderID, float newValue);
};

#endif // SHADEROPTIONSSTACKEDWIDGET_H
