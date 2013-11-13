#ifndef FORT015_NEW_H
#define FORT015_NEW_H

#include <set>
#include <fstream>
#include <ostream>
#include <sstream>

#include <QObject>

#include "NewProjectModel/Files/ProjectFile_new.h"

class Fort015_new : public QObject
{
		Q_OBJECT
	public:
		explicit Fort015_new(QObject *parent=0);
		Fort015_new(ProjectFile_new *projectFile, QObject *parent=0);
		Fort015_new(QString domainName, ProjectFile_new *projectFile, QObject *parent=0);
//		Fort015_new(QString domainName, ProjectFile_new *projectFile, QString targetDir, QObject *parent=0);

//		void	AddInnerBoundaryNodes(std::vector<unsigned int> newNodes);
//		void	AddOuterBoundaryNodes(std::vector<unsigned int> newNodes);
//		void	SetApproach(int approach);
//		void	SetRecordFrequency(int frequency);
//		void	WriteFile();

		void			AddBoundaryNodes(std::vector<unsigned int> newNodes);
		void			AddInnerBoundaryNodes(std::vector<unsigned int> newNodes);
		void			AddOuterBoundaryNodes(std::vector<unsigned int> newNodes);
		bool			FileExists();
		std::set<unsigned int>	GetBoundaryNodes();
		std::set<unsigned int>	GetInnerBoundaryNodes();
		std::set<unsigned int>	GetOuterBoundaryNodes();
		int			GetRecordFrequency();
		int			GetSubdomainApproach();
		void			SetSubdomainApproach(int approach);
		void			SetRecordFrequency(int frequency);
		void			WriteFile();

	private:

//		QString			domainName;
//		std::set<unsigned int>	innerBoundaryNodes;
//		bool			isFullDomainFile;
//		std::set<unsigned int>	outerBoundaryNodes;
//		ProjectFile_new*	projectFile;
//		int			recordFrequency;
//		int			subdomainApproach;

		QString			domainName;
		std::set<unsigned int>	boundaryNodes;
		int			enforceBN;
		std::set<unsigned int>	innerBoundaryNodes;
		bool			isFullDomainFile;
		std::set<unsigned int>	outerBoundaryNodes;
		ProjectFile_new*	projectFile;
		int			recordFrequency;
		int			subdomainApproach;
		QString			targetFile;

//		QString	GetFilePath();
		void	ReadFile();
};

#endif // FORT015_NEW_H
