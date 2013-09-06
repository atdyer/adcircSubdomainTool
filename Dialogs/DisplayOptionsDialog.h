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

	signals:

		void	domainSolidOutlineColorChanged(unsigned int, QColor);
		void	domainSolidFillColorChanged(unsigned int, QColor);
		void	domainGradientOutlineChanged(unsigned int, QGradientStops);
		void	domainGradientFillChanged(unsigned int, QGradientStops);

	private slots:

		void	solidColorChanged(QColor color);
		void	gradientChanged(QGradientStops gradientStops);
};

#endif // DISPLAYOPTIONSDIALOG_H
