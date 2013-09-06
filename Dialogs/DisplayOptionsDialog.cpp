#include "DisplayOptionsDialog.h"
#include "ui_DisplayOptionsDialog.h"

DisplayOptionsDialog::DisplayOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DisplayOptionsDialog)
{
	ui->setupUi(this);

	ui->outlineShaderOptions->setCurrentIndex(ui->outlineShaderType->currentIndex());

	connect(ui->outlineShaderType, SIGNAL(currentIndexChanged(int)), ui->outlineShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->outlineShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->outlineShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));
	connect(ui->fillShaderType, SIGNAL(currentIndexChanged(int)), ui->fillShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->fillShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->fillShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));

}

DisplayOptionsDialog::~DisplayOptionsDialog()
{
	delete ui;
}


void DisplayOptionsDialog::solidColorChanged(QColor color)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 0)
	{
		emit domainSolidOutlineColorChanged(0, color);
	}
	else if (currentTab == 1)
	{
		emit domainSolidFillColorChanged(0, color);
	}
}


void DisplayOptionsDialog::gradientChanged(QGradientStops gradientStops)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 0)
	{
		emit domainGradientOutlineChanged(0, gradientStops);
	}
	else if (currentTab == 1)
	{
		emit domainGradientFillChanged(0, gradientStops);
	}
}
