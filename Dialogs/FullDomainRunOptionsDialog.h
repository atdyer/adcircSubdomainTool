#ifndef FULLDOMAINRUNOPTIONSDIALOG_H
#define FULLDOMAINRUNOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
	class FullDomainRunOptionsDialog;
}

class FullDomainRunOptionsDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit FullDomainRunOptionsDialog(QWidget *parent = 0);
		~FullDomainRunOptionsDialog();

		int	GetSubdomainApproach();
		int	GetRecordFrequency();
		int	GetRunEnvironment();

		
	private:
		Ui::FullDomainRunOptionsDialog *ui;
};

#endif // FULLDOMAINRUNOPTIONSDIALOG_H
