#ifndef FULLDOMAINRUNNER_NEW_H
#define FULLDOMAINRUNNER_NEW_H

#include <QString>
#include <QMessageBox>
#include <QProcess>
#include <iostream>

#include "Dialogs/FullDomainRunOptionsDialog.h"

#include "NewProjectModel/Domains/FullDomain.h"

class FullDomainRunner_new
{
	public:
		FullDomainRunner_new();

		void	SetAdcircExecutable(QString newLoc);
		void	SetFullDomain(FullDomain *newFull);

		bool	PrepareForFullDomainRun();
		bool	PerformFullDomainRun();

	private:

		QString		adcircExecutableLocation;
		QString		adcircExecutableName;
		QStringList	arguments;
		FullDomain*	fullDomain;
		QString		fullDomainPath;
		int		runEnvironment;

		bool	CheckForRequiredFiles();
		bool	CheckForFile(QString fileName);

};

#endif // FULLDOMAINRUNNER_NEW_H
