#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	delegate = 0;

	ui->colorFrame->setAutoFillBackground(true);

	connect(ui->colorPicker, SIGNAL(colorPicked(QColor)), ui->valueSlider, SLOT(setColor(QColor)));
	connect(ui->valueSlider, SIGNAL(colorPicked(QColor)), this, SLOT(colorChanged(QColor)));
	connect(ui->valueSlider, SIGNAL(valuePicked(int)), ui->colorPicker, SLOT(valueSliderChanged(int)));
	connect(ui->hueSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setHue(int)));
	connect(ui->satSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setSaturation(int)));
	connect(ui->valSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setValue(int)));

	connect(ui->basicColorsWidget, SIGNAL(colorClicked(QColor)), ui->colorPicker, SLOT(setColor(QColor)));

	connect(ui->addToCustomColorsButton, SIGNAL(clicked()), this, SLOT(addCurrentColorToCustomColors()));

	/* Set up gradient picker */
	SetupGradientPicker();
}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
}


void ShaderOptionsStackedWidget::SetupGradientPicker()
{
	delegate = new SliderItemDelegate();
	delegate->SetValueRange(ui->gradientSliderWidget->GetMinValue(), ui->gradientSliderWidget->GetMaxValue());
	connect(delegate, SIGNAL(valueChanged(int,float)), this, SLOT(sliderValueSet(int,float)));

	ui->sliderList->setItemDelegate(delegate);
	ui->currentColorButton->SetBackgroundColor(QColor::fromRgb(0, 0, 0));

	connect(ui->gradientSliderWidget, SIGNAL(currentSliderChanged(uint,float,QColor)), this, SLOT(currentSliderChanged(uint,float,QColor)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderColorChanged(uint,QColor)), this, SLOT(gradientSliderColorChanged(uint,QColor)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderValueChanged(uint,float)), this, SLOT(gradientSliderValueChanged(uint,float)));
	connect(ui->gradientSliderWidget, SIGNAL(sliderAdded(uint,float,QColor)), this, SLOT(sliderAdded(uint,float,QColor)));
	connect(ui->addSliderButton, SIGNAL(clicked()), this, SLOT(addSlider()));

	/* Add default sliders */
	unsigned int initialBottomSlider = ui->gradientSliderWidget->AddSlider();
	unsigned int initialTopSlider = ui->gradientSliderWidget->AddSlider();

	if (initialBottomSlider)
	{
		ui->gradientSliderWidget->SetSliderColor(initialBottomSlider, QColor::fromRgb(0, 255, 0));
		ui->gradientSliderWidget->SetSliderValue(initialBottomSlider, ui->gradientSliderWidget->GetMinValue());
	}

	if (initialTopSlider)
	{
		ui->gradientSliderWidget->SetSliderColor(initialTopSlider, QColor::fromRgb(0, 0, 255));
		ui->gradientSliderWidget->SetSliderValue(initialTopSlider, ui->gradientSliderWidget->GetMaxValue());
	}
}


void ShaderOptionsStackedWidget::colorChanged(const QColor &c)
{
	QPalette newPalette (backgroundRole(), c);
	ui->colorFrame->setPalette(newPalette);

	bool hue = ui->hueSpin->blockSignals(true);
	bool sat = ui->satSpin->blockSignals(true);
	bool val = ui->valSpin->blockSignals(true);

	ui->hueSpin->setValue(c.hue());
	ui->satSpin->setValue(c.saturation());
	ui->valSpin->setValue(c.value());

	ui->hueSpin->blockSignals(hue);
	ui->satSpin->blockSignals(sat);
	ui->valSpin->blockSignals(val);
}


void ShaderOptionsStackedWidget::addCurrentColorToCustomColors()
{
	ui->customColorsWidget->addColor(ui->colorFrame->palette().color(backgroundRole()));
}


void ShaderOptionsStackedWidget::addSlider()
{
	unsigned int sliderID = ui->gradientSliderWidget->AddSlider();
	if (sliderID)
	{
		float initialValue = (ui->gradientSliderWidget->GetMinValue() + ui->gradientSliderWidget->GetMaxValue()) / 2.0;
		QColor initialColor = QColor::fromRgb(255, 255, 255);
		ui->gradientSliderWidget->SetSliderValue(sliderID, initialValue);
		ui->gradientSliderWidget->SetSliderColor(sliderID, initialColor);
	}
}


void ShaderOptionsStackedWidget::sliderAdded(unsigned int sliderID, float sliderValue, QColor sliderColor)
{
	if (!sliderListItems.contains(sliderID))
	{
		QListWidgetItem *newItem = new QListWidgetItem(ui->sliderList, 0);

		sliderListItems[sliderID] = newItem;

		newItem->setData(Qt::DisplayRole, QString::number(sliderValue));
		newItem->setData(Qt::EditRole, QString::number(sliderValue));
		newItem->setData(Qt::BackgroundRole, sliderColor);
		newItem->setFlags(newItem->flags() | Qt::ItemIsEditable);
	}
}


void ShaderOptionsStackedWidget::sliderValueSet(int row, float newValue)
{
	unsigned int sliderID = sliderListItems.key(ui->sliderList->item(row), 0);
	ui->gradientSliderWidget->SetSliderValue(sliderID, newValue);
}


void ShaderOptionsStackedWidget::removeSlider(unsigned int sliderID)
{

}


void ShaderOptionsStackedWidget::currentSliderChanged(unsigned int sliderID, float sliderValue, QColor sliderColor)
{
	ui->currentValueLine->setText(QString::number(sliderValue, 'f', 4));
	ui->currentColorButton->SetBackgroundColor(sliderColor);
}


void ShaderOptionsStackedWidget::gradientSliderColorChanged(unsigned int sliderID, QColor newColor)
{
	if (sliderListItems.contains(sliderID))
	{
		QListWidgetItem *listItem = sliderListItems[sliderID];
		listItem->setData(Qt::BackgroundRole, newColor);
	}
}


void ShaderOptionsStackedWidget::gradientSliderValueChanged(unsigned int sliderID, float newValue)
{
	ui->currentValueLine->setText(QString::number(newValue, 'f', 4));
	if (sliderListItems.contains(sliderID))
	{
		QListWidgetItem *listItem = sliderListItems[sliderID];
		listItem->setText(QString::number(newValue));
		ui->sliderList->sortItems(Qt::DescendingOrder);
	}
}
