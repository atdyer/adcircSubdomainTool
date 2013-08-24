#include "CreateProjectDialog.h"
#include "ui_CreateProjectDialog.h"

CreateProjectDialog::CreateProjectDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::CreateProjectDialog)
{
	ui->setupUi(this);

	connect(ui->projectName, SIGNAL(textChanged(QString)), this, SLOT(OnProjectNameChange(QString)));
	connect(ui->projectDirectory, SIGNAL(textChanged(QString)), this, SLOT(OnProjectDirectoryChange(QString)));
	connect(ui->browseButton, SIGNAL(clicked()), this, SLOT(OnBrowseButtonClick()));

	ui->projectDirectory->setText(QDir::toNativeSeparators((QDir::homePath() + QDir::separator())));

	projectDirectory = ui->projectDirectory->text();
	projectName = ui->projectName->text();

	CheckForExistingProject();

	ui->projectName->selectAll();
}

CreateProjectDialog::~CreateProjectDialog()
{
	delete ui;
}


QString CreateProjectDialog::GetProjectDirectory()
{
	return projectDirectory;
}


QString CreateProjectDialog::GetProjectName()
{
	return projectName;
}


void CreateProjectDialog::CheckForExistingProject()
{
	if (ui->projectName->text().isEmpty())
	{
		ui->warnLabel->setText("Choose a Project Name");
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		return;
	}

	if (ProjectDirExists())
	{
		if (ProjectFileExists())
		{
			ui->warnLabel->setText("Project Already Exists");
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);

		} else {
			ui->warnLabel->clear();
			ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
		}
	} else {
		ui->warnLabel->clear();
		ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
	}
}


bool CreateProjectDialog::ProjectDirExists()
{
	return QDir(projectDirectory+projectName).exists();
}


bool CreateProjectDialog::ProjectFileExists()
{
	QStringList filters ("*.spf");
	QDir dir (projectDirectory+projectName);
	dir.setNameFilters(filters);

	if (dir.entryList().size() == 0)
		return false;
	return true;
}


void CreateProjectDialog::OnProjectNameChange(QString projName)
{
	projectName = projName;
	CheckForExistingProject();
}


void CreateProjectDialog::OnProjectDirectoryChange(QString projDir)
{
	projectDirectory = projDir;
	CheckForExistingProject();
}


void CreateProjectDialog::OnBrowseButtonClick()
{
	QStringList selections;
	QFileDialog dialog(this, "Choose the Directory Where the New Project Will Be Created");
	dialog.setFileMode(QFileDialog::Directory);
	dialog.setDirectory(projectDirectory);
	dialog.setModal(true);
	if (dialog.exec())
	{
		selections = dialog.selectedFiles();
		if (!selections.isEmpty())
		{
			QDir dir(selections.first());
			if (dir.exists())
			{
				ui->projectDirectory->setText(dir.path()+QDir::separator());
			}
		}
	}
}
