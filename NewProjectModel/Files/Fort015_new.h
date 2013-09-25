#ifndef FORT015_NEW_H
#define FORT015_NEW_H

#include <set>
#include <fstream>
#include <ostream>

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort015_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort015_new(QObject *parent=0);
		Fort015_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);

		void	AddInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void	AddOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		void	SetApproach(int approach);
		void	SetRecordFrequency(int frequency);
		void	WriteFile();

	private:

		QString			domainName;
		std::set<unsigned int>	innerBoundaryNodes;
		bool			isFullDomainFile;
		std::set<unsigned int>	outerBoundaryNodes;
		ProjectFile_new*	projectFile;
		int			recordFrequency;
		int			subdomainApproach;

		QString	GetFilePath();
};

#endif // FORT015_NEW_H
