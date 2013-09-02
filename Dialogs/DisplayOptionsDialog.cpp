#include "DisplayOptionsDialog.h"
#include "ui_DisplayOptionsDialog.h"

DisplayOptionsDialog::DisplayOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DisplayOptionsDialog)
{
	ui->setupUi(this);

	ui->stackedWidget->setCurrentIndex(ui->outlineShaderType->currentIndex());

	connect(ui->outlineShaderType, SIGNAL(currentIndexChanged(int)), ui->stackedWidget, SLOT(setCurrentIndex(int)));
}

DisplayOptionsDialog::~DisplayOptionsDialog()
{
	delete ui;
}
