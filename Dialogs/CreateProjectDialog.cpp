#include "CreateProjectDialog.h"
#include "ui_CreateProjectDialog.h"

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateProjectDialog)
{
	ui->setupUi(this);
}

CreateProjectDialog::~CreateProjectDialog()
{
	delete ui;
}
