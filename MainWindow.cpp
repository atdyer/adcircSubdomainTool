#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	ui->setupUi(this);

	ui->toolBox->setCurrentIndex(0);
	ui->toolBox->setMinimumWidth(ui->projectTree->width()+6);

	testDomain = 0;
	testProject = 0;
	displayOptionsDialog = 0;

	// Create GLPanel status bar and all labels
	glStatusBar = new QStatusBar();

	mouseXLabel = new QLabel("<b>X:</b> - ");
	mouseYLabel = new QLabel("<b>Y:</b> - ");

	numNodesLabel = new QLabel("<b>Nodes:</b> -       ");
	numElementsLabel = new QLabel("<b>Elements:</b> -       ");
	numTSLabel = new QLabel("<b>Timesteps:</b> -");

	// Stylize GL Panel status bar stuff
	glStatusBar->setSizeGripEnabled(false);
	glStatusBar->setStyleSheet("QStatusBar::item {border: 0px solid black };");

	// Add it all to the UI
	ui->GLPanelStatusLayout->insertWidget(0, glStatusBar);
	glStatusBar->insertPermanentWidget(0, mouseXLabel);
	glStatusBar->insertPermanentWidget(1, mouseYLabel);
	glStatusBar->insertWidget(0, numNodesLabel);
	glStatusBar->insertWidget(1, numElementsLabel);
	glStatusBar->insertWidget(2, numTSLabel);

	// Hide the progress bar and bottom section
	ui->progressBar->hide();
//	ui->mainTabWidget->hide();
//	ui->minimizeMainTabButton->setIcon(QIcon::fromTheme("zoom-in"));

	// Expand the project tree
	ui->projectTree->expandAll();

	/* Create the system tray icon */
	trayIcon = 0;
	trayIconMenu = 0;
//	CreateSystemTrayIcon();

}

MainWindow::~MainWindow()
{
//	if (testDomain)
//		delete testDomain;
	if (testProject)
		delete testProject;
	delete ui;

	CheckForMemoryLeaks();
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
	if (event->key() == Qt::Key_E)
		ui->toolBox->setCurrentWidget(ui->createSubdomainPage);
	if (testDomain)
		testDomain->KeyPress(event);
}


/**
 * @brief Displays output to the text box in the Output tab
 * @param The text (or rich HTML) to be displayed
 */
void MainWindow::displayOutput(QString text)
{
//	ui->outputBox->append(text);
}


void MainWindow::showMouseX(float newX)
{
	if (mouseXLabel)
		mouseXLabel->setText(QString("<b>X:</b> ").append(QString::number(newX, 'f', 4)).append("   "));
}


void MainWindow::showMouseY(float newY)
{
	if (mouseYLabel)
		mouseYLabel->setText(QString("<b>Y:</b> ").append(QString::number(newY, 'f', 4)).append("   "));
}


/**
 * @brief Updates the GL Panel status bar with a  new number of nodes
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


void MainWindow::showNumSelectedNodes(int numNodes){
//	ui->subNodesCount->setText(QString::number(numNodes));
}


void MainWindow::showNumSelectedElements(int numElements)
{
//	ui->subElementsCount->setText(QString::number(numElements));
}


void MainWindow::showCircleStats(float x, float y, float rad)
{
	if (glStatusBar)
		glStatusBar->showMessage(QString("Center: ").append(QString::number(x, 'f', 5)).append(", ").append(QString::number(y, 'f', 5)).append("   Radius: ").append(QString::number(rad, 'f', 5)));
}


void MainWindow::updateVisibleDomain()
{
	if (testProject && testProject->ProjectIsOpen())
	{
		Domain *nextDomain = testProject->GetActiveDomain();
		ConnectNewDomain(nextDomain);
		testDomain = nextDomain;
		ui->GLPanel->SetActiveDomain(testDomain);
	}
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
//	if (currentRow >= 0 && currentRow + 1 < ui->plottingStack->count())
//		ui->plottingStack->setCurrentIndex(currentRow+1);
}


void MainWindow::on_minimizeMainTabButton_clicked()
{
//	if (ui->mainTabWidget->isVisible())
//	{
//		ui->mainTabWidget->hide();
//		ui->minimizeMainTabButton->setIcon(QIcon::fromTheme("zoom-in"));
//	} else {
//		ui->mainTabWidget->show();
//		ui->minimizeMainTabButton->setIcon(QIcon::fromTheme("zoom-out"));
//	}
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
//	if (testDomain)
//		delete testDomain;
//	testDomain = new Domain();
//	testDomain->SetProgressBar(ui->progressBar);
//	ui->GLPanel->SetActiveDomain(testDomain);

//	testDomain->SetFort14Location(QFileDialog::getOpenFileName(this, "Open File", "/home/tristan/Desktop/", "ADCIRC Files (*.14 *.15 *.63 *.64)").toStdString());
////	testDomain->SetFort14Location("/home/tristan/Desktop/adcSwan/sub/fort.14");
////	testDomain->SetFort14Location("/home/tristan/Desktop/fort.14");

//	connect(testDomain, SIGNAL(Message(QString)), this, SLOT(displayOutput(QString)));
//	connect(testDomain, SIGNAL(Instructions(QString)), ui->statusBar, SLOT(showMessage(QString)));
//	connect(testDomain, SIGNAL(ToolFinishedDrawing()), ui->statusBar, SLOT(clearMessage()));
//	connect(testDomain, SIGNAL(MouseX(float)), this, SLOT(showMouseX(float)));
//	connect(testDomain, SIGNAL(MouseY(float)), this, SLOT(showMouseY(float)));
//	connect(testDomain, SIGNAL(CircleToolStatsSet(float,float,float)), this, SLOT(showCircleStats(float,float,float)));
//	connect(testDomain, SIGNAL(NumNodesDomain(int)), this, SLOT(showNumNodes(int)));
//	connect(testDomain, SIGNAL(NumElementsDomain(int)), this, SLOT(showNumElements(int)));
//	connect(testDomain, SIGNAL(NumNodesSelected(int)), this, SLOT(showNumSelectedNodes(int)));
//	connect(testDomain, SIGNAL(NumElementsSelected(int)), this, SLOT(showNumSelectedElements(int)));
//	connect(testDomain, SIGNAL(ToolFinishedDrawing()), glStatusBar, SLOT(clearMessage()));
//	connect(testDomain, SIGNAL(EmitMessage(QString)), this, SLOT(displayOutput(QString)));

//	/* Hook up undo/redo buttons */
//	connect(testDomain, SIGNAL(UndoAvailable(bool)), ui->undoButton, SLOT(setEnabled(bool)));
//	connect(testDomain, SIGNAL(RedoAvailable(bool)), ui->redoButton, SLOT(setEnabled(bool)));
}


void MainWindow::on_newProjectButton_clicked()
{
	if (!testProject)
	{
		testProject = new Project();
		testProject->SetProgressBar(ui->progressBar);
		ConnectProject(testProject);
		testProject->SetProjectTree(ui->projectTree);
		testProject->CreateProject();
		if (testProject->ProjectIsOpen())
			DEBUG("Project successfully created");
		else
			DEBUG("No project created");
	}
}

void MainWindow::on_openProjectButton_clicked()
{
	if (!testProject)
	{
		testProject = new Project();
		testProject->SetProgressBar(ui->progressBar);
		ConnectProject(testProject);
		testProject->SetProjectTree(ui->projectTree);
		testProject->OpenProject();
	}

}

void MainWindow::on_undoButton_clicked()
{
	if (testDomain)
		testDomain->Undo();
}

void MainWindow::on_redoButton_clicked()
{
	if (testDomain)
		testDomain->Redo();
}

void MainWindow::on_selectNodesCircle_clicked()
{
	if (testDomain)
		testDomain->UseTool(CircleToolType, ElementSelection);
}

void MainWindow::on_selectNodesSquare_clicked()
{
	if (testDomain)
		testDomain->UseTool(RectangleToolType, ElementSelection);
}

void MainWindow::on_selectNodeSingle_clicked()
{
	if (testDomain)
		testDomain->UseTool(PolygonToolType, ElementSelection);
}


void MainWindow::on_selectElementSingle_clicked()
{
	if (testDomain)
		testDomain->UseTool(ClickToolType, ElementSelection);
}


void MainWindow::CreateSystemTrayIcon()
{
	if (!trayIcon)
		trayIcon = new QSystemTrayIcon(this);
	if (!trayIconMenu)
		trayIconMenu = new QMenu(this);

	trayIcon->setContextMenu(trayIconMenu);
	trayIcon->setIcon(QIcon(":/icons/images/icons/16_16/circle.png"));
	trayIcon->show();
}


void MainWindow::ConnectNewDomain(Domain *newDomain)
{
	if (testDomain)
	{
		disconnect(testDomain, SIGNAL(Message(QString)), this, SLOT(displayOutput(QString)));
		disconnect(testDomain, SIGNAL(Instructions(QString)), ui->statusBar, SLOT(showMessage(QString)));
		disconnect(testDomain, SIGNAL(ToolFinishedDrawing()), ui->statusBar, SLOT(clearMessage()));
		disconnect(testDomain, SIGNAL(MouseX(float)), this, SLOT(showMouseX(float)));
		disconnect(testDomain, SIGNAL(MouseY(float)), this, SLOT(showMouseY(float)));
		disconnect(testDomain, SIGNAL(CircleToolStatsSet(float,float,float)), this, SLOT(showCircleStats(float,float,float)));
		disconnect(testDomain, SIGNAL(NumNodesDomain(int)), this, SLOT(showNumNodes(int)));
		disconnect(testDomain, SIGNAL(NumElementsDomain(int)), this, SLOT(showNumElements(int)));
		disconnect(testDomain, SIGNAL(NumNodesSelected(int)), this, SLOT(showNumSelectedNodes(int)));
		disconnect(testDomain, SIGNAL(NumElementsSelected(int)), this, SLOT(showNumSelectedElements(int)));
		disconnect(testDomain, SIGNAL(ToolFinishedDrawing()), glStatusBar, SLOT(clearMessage()));
		disconnect(testDomain, SIGNAL(EmitMessage(QString)), this, SLOT(displayOutput(QString)));
		disconnect(testDomain, SIGNAL(UndoAvailable(bool)), ui->undoButton, SLOT(setEnabled(bool)));
		disconnect(testDomain, SIGNAL(RedoAvailable(bool)), ui->redoButton, SLOT(setEnabled(bool)));
	}

	connect(newDomain, SIGNAL(Message(QString)), this, SLOT(displayOutput(QString)));
	connect(newDomain, SIGNAL(Instructions(QString)), ui->statusBar, SLOT(showMessage(QString)));
	connect(newDomain, SIGNAL(ToolFinishedDrawing()), ui->statusBar, SLOT(clearMessage()));
	connect(newDomain, SIGNAL(MouseX(float)), this, SLOT(showMouseX(float)));
	connect(newDomain, SIGNAL(MouseY(float)), this, SLOT(showMouseY(float)));
	connect(newDomain, SIGNAL(CircleToolStatsSet(float,float,float)), this, SLOT(showCircleStats(float,float,float)));
	connect(newDomain, SIGNAL(NumNodesDomain(int)), this, SLOT(showNumNodes(int)));
	connect(newDomain, SIGNAL(NumElementsDomain(int)), this, SLOT(showNumElements(int)));
	connect(newDomain, SIGNAL(NumNodesSelected(int)), this, SLOT(showNumSelectedNodes(int)));
	connect(newDomain, SIGNAL(NumElementsSelected(int)), this, SLOT(showNumSelectedElements(int)));
	connect(newDomain, SIGNAL(ToolFinishedDrawing()), glStatusBar, SLOT(clearMessage()));
	connect(newDomain, SIGNAL(EmitMessage(QString)), this, SLOT(displayOutput(QString)));
	connect(newDomain, SIGNAL(UndoAvailable(bool)), ui->undoButton, SLOT(setEnabled(bool)));
	connect(newDomain, SIGNAL(RedoAvailable(bool)), ui->redoButton, SLOT(setEnabled(bool)));
}


void MainWindow::ConnectProject(Project *newProject)
{
	if (newProject)
	{
		connect(newProject, SIGNAL(newDomainSelected()), this, SLOT(updateVisibleDomain()));
		connect(newProject, SIGNAL(newDomainSelected()), ui->GLPanel, SLOT(updateGL()));
		connect(ui->createSubdomainButton, SIGNAL(clicked()), newProject, SLOT(createSubdomain()));
		connect(ui->actionProjectSettings, SIGNAL(triggered()), newProject, SLOT(showProjectSettings()));
		connect(ui->runFullDomainButton, SIGNAL(clicked()), newProject, SLOT(runFullDomain()));
	}
}


void MainWindow::CheckForMemoryLeaks()
{
	if (GLShader::GetNumShaders() != 0)
	{
		DEBUG("MEMORY LEAK: " << GLShader::GetNumShaders() << " GLShader objects not deleted");
	}
	if (Layer::GetNumLayers() != 0)
	{
		DEBUG("MEMORY LEAK: " << Layer::GetNumLayers() << " Layer objects not deleted");
	}
	if (GLCamera::GetNumCameras() != 0)
	{
		DEBUG("MEMORY LEAK: " << GLCamera::GetNumCameras() << " GLCamera objects not deleted");
	}
}

void MainWindow::on_actionColor_Options_triggered()
{
//	if (!displayOptionsDialog)
//	{
//		displayOptionsDialog = new DisplayOptionsDialog(this);
//	}

	if (testProject)
	{
		testProject->showDisplayOptions();
	}

//	displayOptionsDialog->show();
}
