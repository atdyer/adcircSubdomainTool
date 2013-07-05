#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	ui->setupUi(this);

	// Create GLPanel status bar
	glStatusBar = new QStatusBar();
	numNodesLabel = new QLabel("<b>Nodes:</b> -       ");
	numElementsLabel = new QLabel("<b>Elements:</b> -       ");
	numTSLabel = new QLabel("<b>Timesteps:</b> -");

	// Stylize GL Panel status bar stuff
	glStatusBar->setSizeGripEnabled(false);
	glStatusBar->setStyleSheet("QStatusBar::item {border: 0px solid black };");

	// Add it all to the UI
	ui->GLPanelStatusLayout->insertWidget(0, glStatusBar);
	glStatusBar->insertWidget(0, numNodesLabel);
	glStatusBar->insertWidget(1, numElementsLabel);
	glStatusBar->insertWidget(2, numTSLabel);

	// Hide the progress bar
	ui->progressBar->hide();

	// Expand the project tree
	ui->projectTree->expandAll();

	// Set up communication between the LayerManager and the GLPanel
	connect(&layerManager, SIGNAL(cameraChanged()), ui->GLPanel, SLOT(updateCurrentCamera()));
	ui->GLPanel->SetLayerManager(&layerManager);

	// Connect all necessary components to the output box
	connect(&layerManager, SIGNAL(emitMessage(QString)), this, SLOT(displayOutput(QString)));

	// Connect everything needed to update the GL Panel and GL Panel status bar
	connect(&layerManager, SIGNAL(updateGL()), ui->GLPanel, SLOT(updateGL()));
	connect(&layerManager, SIGNAL(numNodesChanged(int)), this, SLOT(showNumNodes(int)));
	connect(&layerManager, SIGNAL(numElementsChanged(int)), this, SLOT(showNumElements(int)));
	connect(&layerManager, SIGNAL(numTSChanged(int)), this, SLOT(showNumTS(int)));

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
 * @brief Updates the GL Panel status bar with a new number of nodes
 * @param numNodes Number of nodes
 */
void MainWindow::showNumNodes(int numNodes)
{
	if (numNodesLabel)
		numNodesLabel->setText(QString("<b>Nodes:</b> ").append(QString::number(numNodes)).append("   "));
}


/**
 * @brief Updates the GL Panel status bar with a new number of elements
 * @param numElements Number of elements
 */
void MainWindow::showNumElements(int numElements)
{
	if (numElementsLabel)
		numElementsLabel->setText(QString("<b>Elements:</b> ").append(QString::number(numElements)).append("   "));
}


/**
 * @brief Updates the GL Panel status bar with a new number of timesteps
 * @param numTS Numer of timesteps
 */
void MainWindow::showNumTS(int numTS)
{
	if (numTSLabel)
		numTSLabel->setText(QString("<b>Timesteps:</b> ").append(QString::number(numTS)).append("   "));
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
	layerManager.CreateNewTerrainLayer("/home/tristan/Desktop/adcSwan/sub/fort.14", ui->progressBar);
}


void MainWindow::on_newProjectButton_clicked()
{
	displayOutput("Test");
}
