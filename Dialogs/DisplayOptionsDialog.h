#ifndef DISPLAYOPTIONSDIALOG_H
#define DISPLAYOPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
	class DisplayOptionsDialog;
}

class DisplayOptionsDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit DisplayOptionsDialog(QWidget *parent = 0);
		~DisplayOptionsDialog();
		
	private:
		Ui::DisplayOptionsDialog *ui;
};

#endif // DISPLAYOPTIONSDIALOG_H
