#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <qwt_plot.h>

#include "Layers/LayerManager.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
		
	private slots:
		void on_plottingList_currentRowChanged(int currentRow);

		void on_minimizeMainTabButton_clicked();

		void on_openFileButton_clicked();

		void on_newProjectButton_clicked();

	private:
		Ui::MainWindow *ui;

		// The status bar specific to the GLPanel
		QStatusBar *glStatusBar;

		// The LayerManager
		LayerManager	layerManager;

};

#endif // MAINWINDOW_H
