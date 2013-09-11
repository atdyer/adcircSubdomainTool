#include "ProjectSettings.h"

ProjectSettings::ProjectSettings()
{
	adcircExecutableLocation = "";
}


ProjectSettings::~ProjectSettings()
{

}


void ProjectSettings::ShowProjectSettingsDialog()
{
	ReadSettingsFromFile();
	ProjectSettingsDialog dlg;
	dlg.SetAdcircExecutableLocation(adcircExecutableLocation);
	if (dlg.exec())
	{
		adcircExecutableLocation = dlg.GetAdcircExecutableLocation();
	}
}


void ProjectSettings::SetProjectFile(ProjectFile *newFile)
{
	if (newFile)
	{
		projectFile = newFile;
		ReadSettingsFromFile();
	}
}


QString ProjectSettings::GetAdcircExecutableLocation()
{
	ReadSettingsFromFile();
	return adcircExecutableLocation;
}


void ProjectSettings::ReadSettingsFromFile()
{
	if (projectFile)
	{
		if (lastDataFetch.isNull() || lastDataFetch < projectFile->GetLastFileAccess())
		{
			adcircExecutableLocation = projectFile->GetAdcircLocation();

			lastDataFetch = QDateTime::currentDateTime();
		}
	}

}
