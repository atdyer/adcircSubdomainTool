#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include <QLabel>
//#include <qwt_plot.h>

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



	public slots:

		void	displayOutput(QString text);
		void	showMouseX(float newX);
		void	showMouseY(float newY);
		void	showNumNodes(int numNodes);
		void	showNumElements(int numElements);
		void	showNumTS(int numTS);

	private slots:

		// User interface event handlers
		void on_plottingList_currentRowChanged(int currentRow);
		void on_minimizeMainTabButton_clicked();
		void on_openFileButton_clicked();
		void on_newProjectButton_clicked();

	private:

		// The Main Window
		Ui::MainWindow *ui;

		// The status bar specific to the GLPanel
		QStatusBar	*glStatusBar;
		QLabel		*mouseXLabel, *mouseYLabel, *numNodesLabel, *numElementsLabel, *numTSLabel;

		// The LayerManager
		LayerManager	layerManager;

};

#endif // MAINWINDOW_H
