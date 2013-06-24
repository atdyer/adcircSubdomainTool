#ifndef DIALOG_OPENFILE_H
#define DIALOG_OPENFILE_H

#include <QDialog>

namespace Ui {
	class Dialog_OpenFile;
}

class Dialog_OpenFile : public QDialog
{
		Q_OBJECT
		
	public:
		explicit Dialog_OpenFile(QWidget *parent = 0);
		~Dialog_OpenFile();
		
	private:
		Ui::Dialog_OpenFile *ui;
};

#endif // DIALOG_OPENFILE_H
