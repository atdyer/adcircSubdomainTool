#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "Projects/ProjectFile.h"
#include "Dialogs/ProjectSettingsDialog.h"

#include <QDateTime>
#include <iostream>

class ProjectSettings
{
	public:
		ProjectSettings();
		~ProjectSettings();

		void	ShowProjectSettingsDialog();

		void	SetProjectFile(ProjectFile *newFile);

		QString		GetAdcircExecutableLocation();

	private:

		ProjectFile*	projectFile;

		QDateTime	lastDataFetch;
		QString		adcircExecutableLocation;

		void	ReadSettingsFromFile();

};

#endif // PROJECTSETTINGS_H
