#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QFileDialog>
#include <QKeyEvent>
//#include <qwt_plot.h>

#include "Domains/Domain.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT
		
	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:

		void	keyPressEvent(QKeyEvent *event);


	public slots:

		void	displayOutput(QString text);
		void	showMouseX(float newX);
		void	showMouseY(float newY);
		void	showNumNodes(int numNodes);
		void	showNumElements(int numElements);
		void	showNumTS(int numTS);
		void	showNumSelectedNodes(int numNodes);
		void	showNumSelectedElements(int numElements);

		// Circle Tool Slots
		void	showCircleStats(float x, float y, float rad);

	private slots:

		// User interface event handlers
		void on_plottingList_currentRowChanged(int currentRow);
		void on_minimizeMainTabButton_clicked();
		void on_openFileButton_clicked();
		void on_newProjectButton_clicked();
		void on_openProjectButton_clicked();
		void on_undoButton_clicked();
		void on_redoButton_clicked();
		void on_selectNodesCircle_clicked();
		void on_selectNodesSquare_clicked();
		void on_selectNodeSingle_clicked();

	private:

		// The Main Window
		Ui::MainWindow	*ui;

		// The status bar specific to the GLPanel
		QStatusBar	*glStatusBar;
		QLabel		*mouseXLabel, *mouseYLabel, *numNodesLabel, *numElementsLabel, *numTSLabel;

		// The test Domain
		Domain*		testDomain;


		void	CheckForMemoryLeaks();

};

#endif // MAINWINDOW_H
