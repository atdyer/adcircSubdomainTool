#include "ProjectSettings.h"

ProjectSettings::ProjectSettings()
{

}


ProjectSettings::~ProjectSettings()
{

}


void ProjectSettings::ShowProjectSettingsDialog()
{

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
	return adcircExecutableLocation;
}
