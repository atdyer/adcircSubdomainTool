#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	ui->setupUi(this);

	// Create GLPanel status bar
	glStatusBar = new QStatusBar();
	glStatusBar->setSizeGripEnabled(false);
	ui->GLPanelStatusLayout->insertWidget(0, glStatusBar);
	glStatusBar->showMessage("Nodes: -          Elements: -          Timesteps: -");

	// Hide the progress bar
	ui->progressBar->hide();

	// Expand the project tree
	ui->projectTree->expandAll();

	// Start out maximized
	// this->showMaximized();




}

MainWindow::~MainWindow()
{
	delete ui;
}


/**
 * @brief Event Handler: Row is changed in the plotting tools list
 *
 * Changes the visible display in the plottingStack widget corresponding to the newly selected item.
 *
 * @param currentRow The newly selected row
 */
void MainWindow::on_plottingList_currentRowChanged(int currentRow)
{
	if (currentRow >= 0 && currentRow + 1 < ui->plottingStack->count())
		ui->plottingStack->setCurrentIndex(currentRow+1);
}


void MainWindow::on_minimizeMainTabButton_clicked()
{
	if (ui->mainTabWidget->isVisible())
	{
		ui->mainTabWidget->hide();
		ui->minimizeMainTabButton->setIcon(QIcon::fromTheme("zoom-in"));
	} else {
		ui->mainTabWidget->show();
		ui->minimizeMainTabButton->setIcon(QIcon::fromTheme("zoom-out"));
	}
}


void MainWindow::on_openFileButton_clicked()
{
	std::cout << "Main thread: " << this->thread()->currentThreadId() << std::endl;
	layerManager.CreateNewTerrainLayer("hello", ui->progressBar);
}


void MainWindow::on_newProjectButton_clicked()
{

}
