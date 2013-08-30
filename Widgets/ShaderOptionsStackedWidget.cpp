#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	ui->colorFrame->setAutoFillBackground(true);

	connect(ui->colorPicker, SIGNAL(colorPicked(QColor)), ui->valueSlider, SLOT(setColor(QColor)));
	connect(ui->valueSlider, SIGNAL(colorPicked(QColor)), this, SLOT(colorChanged(QColor)));
	connect(ui->valueSlider, SIGNAL(valuePicked(int)), ui->colorPicker, SLOT(valueSliderChanged(int)));
	connect(ui->hueSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setHue(int)));
	connect(ui->satSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setSaturation(int)));
	connect(ui->valSpin, SIGNAL(valueChanged(int)), ui->colorPicker, SLOT(setValue(int)));

	connect(ui->basicColorsWidget, SIGNAL(colorClicked(QColor)), ui->colorPicker, SLOT(setColor(QColor)));
}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
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
