#include "FullDomainRunOptionsDialog.h"
#include "ui_FullDomainRunOptionsDialog.h"

FullDomainRunOptionsDialog::FullDomainRunOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::FullDomainRunOptionsDialog)
{
	ui->setupUi(this);
}

FullDomainRunOptionsDialog::~FullDomainRunOptionsDialog()
{
	delete ui;
}


int FullDomainRunOptionsDialog::GetSubdomainApproach()
{
	return ui->subdomainMethodSpin->currentIndex();
}


int FullDomainRunOptionsDialog::GetRecordFrequency()
{
	return ui->recordFrequency->value();
}


int FullDomainRunOptionsDialog::GetRunEnvironment()
{
	return ui->runEnvironmentGroup->checkedId();
}
