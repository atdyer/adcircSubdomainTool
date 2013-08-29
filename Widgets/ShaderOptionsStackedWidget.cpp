#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	connect(ui->colorPicker, SIGNAL(colorPicked(QColor)), ui->valueSlider, SLOT(setColor(QColor)));
}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
}
