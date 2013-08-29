#include "ShaderOptionsStackedWidget.h"
#include "ui_ShaderOptionsStackedWidget.h"

ShaderOptionsStackedWidget::ShaderOptionsStackedWidget(QWidget *parent) :
	QStackedWidget(parent),
	ui(new Ui::ShaderOptionsStackedWidget)
{
	ui->setupUi(this);

	ColorGradientFrame *colorGradient = new ColorGradientFrame();
	ui->widgetLayout->addWidget(colorGradient);

//	QColorDialog *colorPicker = new QColorDialog();


//	colorPicker->setOption(QColorDialog::DontUseNativeDialog);
//	colorPicker->setOption(QColorDialog::ShowAlphaChannel);
//	colorPicker->setOption(QColorDialog::NoButtons);

//	colorPicker->exec();

//	ui->widgetLayout->addWidget(colorPicker);

//	QColorDialog::getColor(Qt::white, 0, "test", QColorDialog::NoButtons);

}

ShaderOptionsStackedWidget::~ShaderOptionsStackedWidget()
{
	delete ui;
}
