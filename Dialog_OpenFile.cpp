#include "Dialog_OpenFile.h"
#include "ui_Dialog_OpenFile.h"

Dialog_OpenFile::Dialog_OpenFile(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog_OpenFile)
{
	ui->setupUi(this);
}

Dialog_OpenFile::~Dialog_OpenFile()
{
	delete ui;
}
