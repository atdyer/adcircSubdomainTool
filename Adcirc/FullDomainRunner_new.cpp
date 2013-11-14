#include "FullDomainRunner_new.h"

FullDomainRunner_new::FullDomainRunner_new() :
	adcircExecutableLocation(),
	adcircExecutableName(),
	arguments(),
	fullDomain(0),
	fullDomainPath(),
	runEnvironment(0)
{
}

void FullDomainRunner_new::SetAdcircExecutable(QString newLoc)
{
	adcircExecutableLocation = newLoc;
}


void FullDomainRunner_new::SetFullDomain(FullDomain *newFull)
{
	if (newFull)
	{
		fullDomain = newFull;
		fullDomainPath = fullDomain->GetPath();
	}
}


bool FullDomainRunner_new::PrepareForFullDomainRun()
{
	FullDomainRunOptionsDialog dlg;
	dlg.SetAdcircExecutable(adcircExecutableLocation);
	if (dlg.exec())
	{
		adcircExecutableLocation = dlg.GetAdcircExecutableLocation();
		runEnvironment = dlg.GetRunEnvironment();
		std::cout << "ADCIRC: " << adcircExecutableLocation.toStdString() << std::endl;
		return CheckForRequiredFiles();
	}
	return false;
}


bool FullDomainRunner_new::PerformFullDomainRun()
{
	arguments.clear();
	arguments << "--working-directory="+fullDomainPath;	// Set the working directory of the new terminal
	arguments << "-e";					// Execute command flag
	arguments << "./"+adcircExecutableName;			// Adcirc executable link in project path

	QProcess *bash = new QProcess();
	bash->start("gnome-terminal", arguments);
	return bash->waitForStarted();
}


bool FullDomainRunner_new::CheckForRequiredFiles()
{
	QFile adcExe(adcircExecutableLocation);
	adcircExecutableName = QFileInfo(adcExe).fileName();

	/* Check for fort.14, fort.15, fort.015, ln to adcirc */
	bool fort14 = CheckForFile("fort.14");
	bool fort15 = CheckForFile("fort.15");
	bool fort015 = CheckForFile("fort.015");
	bool adcirc = CheckForFile(adcircExecutableName);

	if (adcExe.exists() && !adcirc)
	{
#ifdef Q_OS_WIN32
		adcirc = adcExe.link(fullDomainPath + QDir::separator() + exeName + ".lnk");
#else
		adcirc = adcExe.link(fullDomainPath + QDir::separator() + adcircExecutableName);
#endif
	}

	if (!adcirc || !fort14 || !fort15 || !fort015)
	{
		QString message ("Error - The following files are missing:\n");
		message.append(adcirc ? "" : QString(fullDomainPath) + QDir::separator() + adcircExecutableName + "\n");
		message.append(fort14 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.14\n");
		message.append(fort15 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.15\n");
		message.append(fort015 ? "" : QString(fullDomainPath) + QDir::separator() + "fort.015");
		QMessageBox dlg;
		dlg.setWindowTitle("Run Full Domain");
		dlg.setText(message);
		dlg.setIcon(QMessageBox::Critical);
		dlg.setStandardButtons(QMessageBox::Ok);
		dlg.exec();
		return false;
	}

	return true;
}


bool FullDomainRunner_new::CheckForFile(QString fileName)
{
	return QFile(fullDomainPath + QDir::separator() + fileName).exists();
}

