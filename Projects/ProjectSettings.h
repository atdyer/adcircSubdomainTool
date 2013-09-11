#ifndef PROJECTSETTINGS_H
#define PROJECTSETTINGS_H

#include "Projects/ProjectFile.h"

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

		QString		adcircExecutableLocation;

		void	ReadSettingsFromFile();

};

#endif // PROJECTSETTINGS_H
