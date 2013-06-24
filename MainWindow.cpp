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

	// Set up communication between the LayerManager and the GLPanel
	connect(&layerManager, SIGNAL(cameraChanged()), ui->GLPanel, SLOT(updateCurrentCamera()));

	// Connect all necessary components to the output box
	connect(&layerManager, SIGNAL(emitMessage(QString)), this, SLOT(displayOutput(QString)));

}

MainWindow::~MainWindow()
{
	delete ui;
}


/**
 * @brief Displays output to the text box in the Output tab
 * @param The text (or rich HTML) to be displayed
 */
void MainWindow::displayOutput(QString text)
{
	ui->outputBox->append(text);
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


/**
  * @brief Event Handler: Open File tool button is clicked
  *
  * Opens a dialog window which is connected to the appropriate slots for
  * handling any number of selected files
  *
  */
void MainWindow::on_openFileButton_clicked()
{
	// Testing the Layer Manager by adding a terrain layer
	layerManager.CreateNewTerrainLayer("/home/tristan/Desktop/fort.14", ui->progressBar);
}


void MainWindow::on_newProjectButton_clicked()
{
	displayOutput("Test");
}
