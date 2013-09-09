#include "DisplayOptionsDialog.h"
#include "ui_DisplayOptionsDialog.h"

DisplayOptionsDialog::DisplayOptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::DisplayOptionsDialog)
{
	ui->setupUi(this);
	setWindowFlags(Qt::WindowStaysOnTopHint);
	currentDomain = 0;

	ui->outlineShaderOptions->setCurrentIndex(ui->outlineShaderType->currentIndex());

	connect(ui->outlineShaderType, SIGNAL(currentIndexChanged(int)), ui->outlineShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->outlineShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->outlineShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));
	connect(ui->fillShaderType, SIGNAL(currentIndexChanged(int)), ui->fillShaderOptions, SLOT(setCurrentIndex(int)));
	connect(ui->fillShaderOptions, SIGNAL(solidColorChanged(QColor)), this, SLOT(solidColorChanged(QColor)));
	connect(ui->fillShaderOptions, SIGNAL(gradientChanged(QGradientStops)), this, SLOT(gradientChanged(QGradientStops)));

}

DisplayOptionsDialog::~DisplayOptionsDialog()
{
	delete ui;
}


void DisplayOptionsDialog::SetActiveDomain(Domain *newDomain)
{
	DisconnectCurrentDomain();
	currentDomain = newDomain;
	DisplayCurrentDomainProperties();
	ConnectCurrentDomain();
}


void DisplayOptionsDialog::DisconnectCurrentDomain()
{
	if (currentDomain)
	{
		disconnect(this, SIGNAL(solidOutlineChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidOutline(QColor)));
		disconnect(this, SIGNAL(solidFillChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidFill(QColor)));
		disconnect(this, SIGNAL(gradientOutlineChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientOutline(QGradientStops)));
		disconnect(this, SIGNAL(gradientFillChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientFill(QGradientStops)));
	}
}


void DisplayOptionsDialog::DisplayCurrentDomainProperties()
{
	if (currentDomain)
	{

		ui->outlineShaderOptions->SetSolidColor(currentDomain->GetTerrainSolidOutline());
		ui->outlineShaderOptions->SetGradientRange(currentDomain->GetTerrainMinElevation(),
							   currentDomain->GetTerrainMaxElevation());
		ui->outlineShaderOptions->SetGradient(currentDomain->GetTerrainGradientOutline());

		ui->fillShaderOptions->SetSolidColor(currentDomain->GetTerrainSolidFill());
		ui->fillShaderOptions->SetGradientRange(currentDomain->GetTerrainMinElevation(),
							currentDomain->GetTerrainMaxElevation());
		ui->fillShaderOptions->SetGradient(currentDomain->GetTerrainGradientFill());

		ShaderType currentFill = currentDomain->GetTerrainFillShaderType();
		ShaderType currentOutline = currentDomain->GetTerrainOutlineShaderType();

		if (currentFill == SolidShaderType)
		{
			showSolidFillWindow();
		}
		else if (currentFill == GradientShaderType)
		{
			showGradientFillWindow();
		}

		if (currentOutline == SolidShaderType)
		{
			showSolidOutlineWindow();
		}
		else if (currentOutline == GradientShaderType)
		{
			showGradientOutlineWindow();
		}
	}
}


void DisplayOptionsDialog::ConnectCurrentDomain()
{
	if (currentDomain)
	{
		connect(this, SIGNAL(solidOutlineChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidOutline(QColor)));
		connect(this, SIGNAL(solidFillChanged(QColor)),
			   currentDomain, SLOT(SetTerrainSolidFill(QColor)));
		connect(this, SIGNAL(gradientOutlineChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientOutline(QGradientStops)));
		connect(this, SIGNAL(gradientFillChanged(QGradientStops)),
			   currentDomain, SLOT(SetTerrainGradientFill(QGradientStops)));
	}
}


void DisplayOptionsDialog::showSolidOutlineWindow()
{
//	ui->tabWidget->setCurrentIndex(1);
	ui->outlineShaderType->setCurrentIndex(0);
//	ui->outlineShaderOptions->setCurrentIndex(0);
}


void DisplayOptionsDialog::showSolidFillWindow()
{
//	ui->tabWidget->setCurrentIndex(0);
	ui->fillShaderType->setCurrentIndex(0);
//	ui->fillShaderOptions->setCurrentIndex(0);
}


void DisplayOptionsDialog::showGradientOutlineWindow()
{
//	ui->tabWidget->setCurrentIndex(1);
	ui->outlineShaderType->setCurrentIndex(1);
//	ui->outlineShaderOptions->setCurrentIndex(1);
}


void DisplayOptionsDialog::showGradientFillWindow()
{
//	ui->tabWidget->setCurrentIndex(0);
	ui->fillShaderType->setCurrentIndex(1);
//	ui->fillShaderOptions->setCurrentIndex(1);
}


void DisplayOptionsDialog::solidColorChanged(QColor color)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 1)
	{
		emit solidOutlineChanged(color);
	}
	else if (currentTab == 0)
	{
		emit solidFillChanged(color);
	}
}


void DisplayOptionsDialog::gradientChanged(QGradientStops gradientStops)
{
	int currentTab = ui->tabWidget->currentIndex();
	if (currentTab == 1)
	{
		emit gradientOutlineChanged(gradientStops);
	}
	else if (currentTab == 0)
	{
		emit gradientFillChanged(gradientStops);
	}
}
