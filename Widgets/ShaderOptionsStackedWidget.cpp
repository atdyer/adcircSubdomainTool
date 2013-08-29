#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	ColorGradientFrame *colorGradient = new ColorGradientFrame();
	colorGradient->setColor(QColor::fromHsv(359, 255, 200));
	ui->widgetLayout->addWidget(colorGradient);
}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
}
