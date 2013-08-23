#ifndef CREATEPROJECTDIALOG_H
#define CREATEPROJECTDIALOG_H

#include <QDialog>
#include <QDir>
#include <QFileDialog>

namespace Ui {
	class CreateProjectDialog;
}

class CreateProjectDialog : public QDialog
{
		Q_OBJECT
		
	public:
		explicit CreateProjectDialog(QWidget *parent = 0);
		~CreateProjectDialog();

		QString	GetProjectDirectory();
		QString	GetProjectName();
		
	private:
		Ui::CreateProjectDialog *ui;

		QString projectDirectory;
		QString projectName;

		void	CheckForExistingProject();
		bool	ProjectDirExists();
		bool	ProjectFileExists();

	private slots:

		void	OnProjectNameChange(QString projName);
		void	OnProjectDirectoryChange(QString projDir);
		void	OnBrowseButtonClick();


};

#endif // CREATEPROJECTDIALOG_H
