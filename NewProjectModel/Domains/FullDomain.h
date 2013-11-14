#ifndef FULLDOMAIN_H
#define FULLDOMAIN_H

#include <iostream>
#include <vector>

#include <QObject>
#include <QProgressBar>

#include "NewProjectModel/Domains/Domain_new.h"

#include "Layers/SelectionLayers/FullDomainSelectionLayer.h"

#include "NewProjectModel/Files/Fort14_new.h"
#include "NewProjectModel/Files/Fort15_new.h"
#include "NewProjectModel/Files/Fort22_new.h"
#include "NewProjectModel/Files/Fort63_new.h"
#include "NewProjectModel/Files/Fort64_new.h"
#include "NewProjectModel/Files/Fort015_new.h"
#include "NewProjectModel/Files/Fort066_new.h"
#include "NewProjectModel/Files/Fort067_new.h"
#include "NewProjectModel/Files/Maxele63_new.h"
#include "NewProjectModel/Files/Maxvel63_new.h"
#include "NewProjectModel/Files/ProjectFile_new.h"

#include "OpenGL/GLCamera.h"


class FullDomain : public Domain_new
{
		Q_OBJECT
	public:
		FullDomain(ProjectFile_new *projectFile, QObject *parent=0);
		~FullDomain();

		virtual bool	IsFullDomain();

		Fort015_new*			GetFort015();
		std::vector<unsigned int>	GetInnerBoundaryNodes();
		std::vector<unsigned int>	GetOuterBoundaryNodes();
		std::vector<Element*>		GetSelectedElements();

	private:

		FullDomainSelectionLayer*	selectionLayerFullDomain;

		Fort15_new*		fort15;
		Fort22_new*		fort22;
		Fort63_new*		fort63;
		Fort64_new*		fort64;
		Fort015_new*		fort015;
		Fort066_new*		fort066;
		Fort067_new*		fort067;
		Maxele63_new*		maxele;
		Maxvel63_new*		maxvel;

		void	CreateAllFiles();

};

#endif // FULLDOMAIN_H
