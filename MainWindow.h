#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Projects/Project.h"
#include "NewProjectModel/Project_new.h"
#include "Domains/Domain.h"

#include "Dialogs/DisplayOptionsDialog.h"
#include "Dialogs/CreateProjectDialog.h"

#include <QMainWindow>
#include <QThread>
#include <QLabel>
#include <QFileDialog>
#include <QKeyEvent>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QGLFormat>
//#include <qwt_plot.h>

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
		void	showMaxSelectedZ(float newZ);
		void	showMinSelectedZ(float newZ);
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

		void	updateVisibleDomain();

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
		void on_selectElementSingle_clicked();

		/* Menu bar actions */
		void on_actionColor_Options_triggered();

		/* Left side pane slots */
		void	showProjectExplorerPane();
		void	showCreateSubdomainPane();
		void	showEditSubdomainPane();
		void	showAdcircPane();
		void	showAnalyzeResultsPane();

	private:

		// The Main Window
		Ui::MainWindow	*ui;

		/* System Tray Icon */
		QSystemTrayIcon	*trayIcon;
		QMenu		*trayIconMenu;
		void		CreateSystemTrayIcon();

		// The status bar specific to the GLPanel
		QStatusBar	*glStatusBar;
		QLabel		*mouseXLabel, *mouseYLabel, *numNodesLabel, *numElementsLabel, *numTSLabel;


		// The test Project
		Project*	testProject;
		Project_new*	newProject;

		// The test Domain
		Domain*		testDomain;

		// Dialogs
		DisplayOptionsDialog*	displayOptionsDialog;

		void	ConnectNewDomain(Domain *newDomain);
		void	ConnectProject(Project *newProject);
		void	CreateProjectNew(bool newProjectFile); // Create the new project object and connect
		void	CheckForMemoryLeaks();

};

#endif // MAINWINDOW_H
