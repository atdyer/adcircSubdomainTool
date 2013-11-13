#include "Project_new.h"

Project_new::Project_new(QObject *parent) :
	QObject(parent),
	fullDomain(0),
	glPanel(0),
	progressBar(0),
	projectFile(0),
	projectTree(0),
	subDomains(),
	visibleDomain(0)
{
	CreateProjectFile();
}


Project_new::Project_new(QString projectFile, QObject *parent) :
	QObject(parent),
	fullDomain(0),
	glPanel(0),
	progressBar(0),
	projectFile(0),
	projectTree(0),
	subDomains(),
	visibleDomain(0)
{
	OpenProjectFile(projectFile);
}


Project_new::~Project_new()
{
	if (projectFile)
		delete projectFile;
}


void Project_new::SetOpenGLPanel(OpenGLPanel *newPanel)
{
	glPanel = newPanel;
}


void Project_new::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;

	if (fullDomain)
		fullDomain->SetProgressBar(newBar);

	SubDomain *currDomain = 0;
	for (std::vector<SubDomain*>::iterator it = subDomains.begin(); it != subDomains.end(); ++it)
	{
		currDomain = *it;
		if (currDomain)
			currDomain->SetProgressBar(newBar);
	}
}


void Project_new::SetProjectTree(QTreeWidget *newTree)
{
	if (newTree)
	{
		projectTree = newTree;
		PopulateProjectTree();
		connect(projectTree, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),
			this, SLOT(ProjectTreeItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));
	}
}


FullDomain* Project_new::BuildFullDomain()
{
	FullDomain *newFullDomain = new FullDomain(projectFile, this);

	connect(newFullDomain, SIGNAL(mouseX(float)), this, SIGNAL(mouseX(float)));
	connect(newFullDomain, SIGNAL(mouseY(float)), this, SIGNAL(mouseY(float)));
	connect(newFullDomain, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
	connect(newFullDomain, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));
	connect(newFullDomain, SIGNAL(numElementsSelected(int)), this, SIGNAL(numElementsSelected(int)));
	connect(newFullDomain, SIGNAL(numNodesSelected(int)), this, SIGNAL(numNodesSelected(int)));
	connect(newFullDomain, SIGNAL(maxSelectedZ(float)), this, SIGNAL(maxSelectedZ(float)));
	connect(newFullDomain, SIGNAL(minSelectedZ(float)), this, SIGNAL(minSelectedZ(float)));

	return newFullDomain;
}


SubDomain* Project_new::BuildSubdomain(QString subdomainName)
{
	SubDomain *newSubdomain = new SubDomain(subdomainName, projectFile, this);

	connect(newSubdomain, SIGNAL(mouseX(float)), this, SIGNAL(mouseX(float)));
	connect(newSubdomain, SIGNAL(mouseY(float)), this, SIGNAL(mouseY(float)));
	connect(newSubdomain, SIGNAL(undoAvailable(bool)), this, SIGNAL(undoAvailable(bool)));
	connect(newSubdomain, SIGNAL(redoAvailable(bool)), this, SIGNAL(redoAvailable(bool)));

	if (progressBar)
		newSubdomain->SetProgressBar(progressBar);

	subDomains.push_back(newSubdomain);
	return newSubdomain;
}


void Project_new::CreateAllSubdomains()
{
	if (projectFile)
	{
		QStringList subdomainNames = projectFile->GetSubDomainNames();
		for (int i = 0; i<subdomainNames.size(); ++i)
		{
			BuildSubdomain(subdomainNames.at(i));
		}
	}
}


void Project_new::CreateProjectFile()
{
	if (!projectFile)
	{
		projectFile = new ProjectFile_new();
		CreateProjectDialog dialog;
		dialog.setModal(true);
		if (dialog.exec() && projectFile)
		{
			if (projectFile->CreateProjectFile_new(dialog.GetProjectDirectory(), dialog.GetProjectName()))
			{
				QString fort10Loc = dialog.GetFort10Location();
				QString fort11Loc = dialog.GetFort11Location();
				QString fort13Loc = dialog.GetFort13Location();
				QString fort14Loc = dialog.GetFort14Location();
				QString fort15Loc = dialog.GetFort15Location();
				QString fort19Loc = dialog.GetFort19Location();
				QString fort20Loc = dialog.GetFort20Location();
				QString fort22Loc = dialog.GetFort22Location();
				QString fort23Loc = dialog.GetFort23Location();
				QString fort24Loc = dialog.GetFort24Location();
				if (!fort10Loc.isEmpty())
					projectFile->SetFullDomainFort10(fort10Loc, dialog.GetUseSymbolicLinkFort10());
				if (!fort11Loc.isEmpty())
					projectFile->SetFullDomainFort11(fort11Loc, dialog.GetUseSymbolicLinkFort11());
				if (!fort13Loc.isEmpty())
					projectFile->SetFullDomainFort13(fort13Loc, dialog.GetUseSymbolicLinkFort13());
				if (!fort14Loc.isEmpty())
					projectFile->SetFullDomainFort14(fort14Loc, dialog.GetUseSymbolicLinkFort14());
				if (!fort15Loc.isEmpty())
					projectFile->SetFullDomainFort15(fort15Loc, dialog.GetUseSymbolicLinkFort15());
				if (!fort19Loc.isEmpty())
					projectFile->SetFullDomainFort19(fort19Loc, dialog.GetUseSymbolicLinkFort19());
				if (!fort20Loc.isEmpty())
					projectFile->SetFullDomainFort20(fort20Loc, dialog.GetUseSymbolicLinkFort20());
				if (!fort22Loc.isEmpty())
					projectFile->SetFullDomainFort22(fort22Loc, dialog.GetUseSymbolicLinkFort22());
				if (!fort23Loc.isEmpty())
					projectFile->SetFullDomainFort23(fort23Loc, dialog.GetUseSymbolicLinkFort23());
				if (!fort24Loc.isEmpty())
					projectFile->SetFullDomainFort24(fort24Loc, dialog.GetUseSymbolicLinkFort24());
				projectFile->SaveProject();
			}
		}
	}
}


Domain_new* Project_new::DetermineSelectedDomain(QTreeWidgetItem *item)
{
	if (item)
	{
		QString itemText = item->data(0, Qt::DisplayRole).toString();
		if (itemText == QString("Full Domain"))
		{
			return fullDomain;
		}
		QTreeWidgetItem *parentItem = item->parent();
		if (parentItem)
		{
			QString parentText = parentItem->data(0, Qt::DisplayRole).toString();
			if (parentText == QString("Full Domain"))
			{
				return fullDomain;
			}
			else if (parentText == QString("Sub Domains"))
			{
				for (std::vector<SubDomain*>::iterator currSub = subDomains.begin();
				     currSub != subDomains.end();
				     ++currSub)
				{
					if ((*currSub)->GetDomainName() == itemText)
						return *currSub;
				}
			}
			else
			{
					return DetermineSelectedDomain(parentItem);
			}
		}
	}

	return 0;
}


void Project_new::OpenProjectFile(QString filePath)
{
	if (!projectFile)
	{
		projectFile = new ProjectFile_new();
		if (!projectFile->OpenProjectFile_new(filePath))
		{
			delete projectFile;
			projectFile = 0;
		} else {
			fullDomain = BuildFullDomain();
			CreateAllSubdomains();
		}
	}
}


void Project_new::PopulateProjectTree()
{
	if (projectTree && projectFile)
	{
		projectTree->clear();
		projectTree->setColumnCount(2);
//		projectTree->setColumnWidth(0, projectTree->width()-20);
//		projectTree->setColumnWidth(1, 20);
		projectTree->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		projectTree->header()->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
		projectTree->header()->setStretchLastSection(false);
		projectTree->header()->resizeSection(0, projectTree->header()->width()-25);
		projectTree->header()->resizeSection(1, 25);

		QString projectName = projectFile->GetProjectName();
		QString projectDir = projectFile->GetProjectDirectory();
		QString fDir = projectFile->GetProjectDirectory();
		QString f14 = projectFile->GetFullDomainFort14();
		QString f15 = projectFile->GetFullDomainFort15();
		QString f63 = projectFile->GetFullDomainFort63();
		QString f64 = projectFile->GetFullDomainFort64();
		QString f015 = projectFile->GetFullDomainFort015();
		QString f065 = projectFile->GetFullDomainFort065();
		QString f066 = projectFile->GetFullDomainFort066();
		QString f067 = projectFile->GetFullDomainFort067();

		QFont boldFont;
		boldFont.setBold(true);

		QTreeWidgetItem *treeTop = new QTreeWidgetItem(projectTree);
		treeTop->setData(0, Qt::DisplayRole, projectName);
		treeTop->setData(0, Qt::FontRole, boldFont);
		treeTop->setData(0, Qt::StatusTipRole, projectName + " - " + projectDir);

		if (!fDir.isEmpty())
		{
			QTreeWidgetItem *fullTop = new QTreeWidgetItem(treeTop);
			fullTop->setData(0, Qt::DisplayRole, QString("Full Domain"));
			fullTop->setData(0, Qt::StatusTipRole, "Full Domain - " + projectDir);
			fullTop->setIcon(1, QIcon(":/icons/images/icons/16_16/check.png"));

			if (!f14.isEmpty())
			{
				QTreeWidgetItem *item14 = new QTreeWidgetItem(fullTop);
				item14->setData(0, Qt::DisplayRole, QString("fort.14"));
				item14->setData(0, Qt::StatusTipRole, f14);
			}

			if (!f15.isEmpty())
			{
				QTreeWidgetItem *item15 = new QTreeWidgetItem(fullTop);
				item15->setData(0, Qt::DisplayRole, QString("fort.15"));
				item15->setData(0, Qt::StatusTipRole, f15);
			}

			if (!f63.isEmpty())
			{
				QTreeWidgetItem *item63 = new QTreeWidgetItem(fullTop);
				item63->setData(0, Qt::DisplayRole, QString("fort.63"));
				item63->setData(0, Qt::StatusTipRole, f63);
			}

			if (!f64.isEmpty())
			{
				QTreeWidgetItem *item64 = new QTreeWidgetItem(fullTop);
				item64->setData(0, Qt::DisplayRole, QString("fort.64"));
				item64->setData(0, Qt::StatusTipRole, f64);
			}

			if (!f015.isEmpty())
			{
				QTreeWidgetItem *item015 = new QTreeWidgetItem(fullTop);
				item015->setData(0, Qt::DisplayRole, QString("fort.015"));
				item015->setData(0, Qt::StatusTipRole, f015);
			}

			if (!f065.isEmpty())
			{
				QTreeWidgetItem *item065 = new QTreeWidgetItem(fullTop);
				item065->setData(0, Qt::DisplayRole, QString("fort.065"));
				item065->setData(0, Qt::StatusTipRole, f065);
			}

			if (!f066.isEmpty())
			{
				QTreeWidgetItem *item066 = new QTreeWidgetItem(fullTop);
				item066->setData(0, Qt::DisplayRole, QString("fort.066"));
				item066->setData(0, Qt::StatusTipRole, f066);
			}

			if (!f067.isEmpty())
			{
				QTreeWidgetItem *item067 = new QTreeWidgetItem(fullTop);
				item067->setData(0, Qt::DisplayRole, QString("fort.067"));
				item067->setData(0, Qt::StatusTipRole, f067);
			}
		}

		QTreeWidgetItem *subTop = new QTreeWidgetItem(treeTop);
		subTop->setData(0, Qt::DisplayRole, "Sub Domains");

		QStringList subDomainNames = projectFile->GetSubDomainNames();
		for (int i=0; i<subDomainNames.size(); ++i)
		{
			QString currName = subDomainNames.at(i);
			QString	currDir = projectFile->GetSubDomainDirectory(currName);
			QString sBN = projectFile->GetSubDomainBNList(currName);
			QString s14 = projectFile->GetSubDomainFort14(currName);
			QString s15 = projectFile->GetSubDomainFort15(currName);
			QString s63 = projectFile->GetSubDomainFort63(currName);
			QString s64 = projectFile->GetSubDomainFort64(currName);
			QString s015 = projectFile->GetSubDomainFort015(currName);
			QString s019 = projectFile->GetSubDomainFort019(currName);
			QString s020 = projectFile->GetSubDomainFort020(currName);
			QString s021 = projectFile->GetSubDomainFort021(currName);
			QString sME = projectFile->GetSubDomainMaxele(currName);
			QString sMV = projectFile->GetSubDomainMaxvel(currName);
			QString s140 = projectFile->GetSubDomainPy140(currName);
			QString s141 = projectFile->GetSubDomainPy141(currName);

			QTreeWidgetItem *currSub = new QTreeWidgetItem(subTop);
			currSub->setData(0, Qt::DisplayRole, currName);
			currSub->setData(0, Qt::StatusTipRole, currName + " - " + currDir);
			currSub->setIcon(1, QIcon(":/icons/images/icons/16_16/refresh.png"));

			if (!sBN.isEmpty())
			{
				QTreeWidgetItem *itemBN = new QTreeWidgetItem(currSub);
				itemBN->setData(0, Qt::DisplayRole, QString("bnList.14"));
				itemBN->setData(0, Qt::StatusTipRole, sBN);
			}

			if (!s14.isEmpty())
			{
				QTreeWidgetItem *item14 = new QTreeWidgetItem(currSub);
				item14->setData(0, Qt::DisplayRole, QString("fort.14"));
				item14->setData(0, Qt::StatusTipRole, s14);
			}

			if (!s15.isEmpty())
			{
				QTreeWidgetItem *item15 = new QTreeWidgetItem(currSub);
				item15->setData(0, Qt::DisplayRole, QString("fort.15"));
				item15->setData(0, Qt::StatusTipRole, s15);
			}

			if (!s63.isEmpty())
			{
				QTreeWidgetItem *item63 = new QTreeWidgetItem(currSub);
				item63->setData(0, Qt::DisplayRole, QString("fort.63"));
				item63->setData(0, Qt::StatusTipRole, s63);
			}

			if (!s64.isEmpty())
			{
				QTreeWidgetItem *item64 = new QTreeWidgetItem(currSub);
				item64->setData(0, Qt::DisplayRole, QString("fort.64"));
				item64->setData(0, Qt::StatusTipRole, s64);
			}

			if (!s015.isEmpty())
			{
				QTreeWidgetItem *item015 = new QTreeWidgetItem(currSub);
				item015->setData(0, Qt::DisplayRole, QString("fort.015"));
				item015->setData(0, Qt::StatusTipRole, s015);
			}

			if (!s019.isEmpty())
			{
				QTreeWidgetItem *item019 = new QTreeWidgetItem(currSub);
				item019->setData(0, Qt::DisplayRole, QString("fort.019"));
				item019->setData(0, Qt::StatusTipRole, s019);
			}

			if (!s020.isEmpty())
			{
				QTreeWidgetItem *item020 = new QTreeWidgetItem(currSub);
				item020->setData(0, Qt::DisplayRole, QString("fort.020"));
				item020->setData(0, Qt::StatusTipRole, s020);
			}

			if (!s021.isEmpty())
			{
				QTreeWidgetItem *item021 = new QTreeWidgetItem(currSub);
				item021->setData(0, Qt::DisplayRole, QString("fort.021"));
				item021->setData(0, Qt::StatusTipRole, s021);
			}

			if (!sME.isEmpty())
			{
				QTreeWidgetItem *itemME = new QTreeWidgetItem(currSub);
				itemME->setData(0, Qt::DisplayRole, QString("maxele.63"));
				itemME->setData(0, Qt::StatusTipRole, sME);
			}

			if (!sMV.isEmpty())
			{
				QTreeWidgetItem *itemMV = new QTreeWidgetItem(currSub);
				itemMV->setData(0, Qt::DisplayRole, QString("maxvel.63"));
				itemMV->setData(0, Qt::StatusTipRole, sMV);
			}

			if (!s140.isEmpty())
			{
				QTreeWidgetItem *item140 = new QTreeWidgetItem(currSub);
				item140->setData(0, Qt::DisplayRole, QString("py.140"));
				item140->setData(0, Qt::StatusTipRole, s140);
			}

			if (!s141.isEmpty())
			{
				QTreeWidgetItem *item141 = new QTreeWidgetItem(currSub);
				item141->setData(0, Qt::DisplayRole, QString("py.141"));
				item141->setData(0, Qt::StatusTipRole, s141);
			}
		}
		projectTree->collapseAll();
		projectTree->expandToDepth(1);
	}
}


void Project_new::SetVisibleDomain(Domain_new *newDomain)
{
	if (newDomain)
	{
		visibleDomain = newDomain;
		if (glPanel)
			glPanel->SetActiveDomainNew(visibleDomain);

		emit undoAvailable(visibleDomain->UndoAvailable());
		emit redoAvailable(visibleDomain->RedoAvailable());
	}
}


void Project_new::CreateNewSubdomain()
{
	if (fullDomain && projectFile)
	{
		bool ok;
		QString newName = QInputDialog::getText(0,
							"Create Subdomain",
							"Please enter a name for the new subdomain: ",
							QLineEdit::Normal,
							"",
							&ok);
		if (ok && !newName.isEmpty())
		{
			SubdomainCreator_new creator;
			QString targetDir = projectFile->GetFullDomainDirectory() + QDir::separator() + newName;
			bool newSubdomain = creator.CreateSubdomain(newName, projectFile, targetDir, fullDomain, 2, 1);
			if (newSubdomain)
			{
				BuildSubdomain(newName);
				PopulateProjectTree();
				emit showProjectView();
				projectTree->setCurrentItem(projectTree->findItems(newName, Qt::MatchExactly | Qt::MatchRecursive).first());
			}
		}
	}
}


void Project_new::EditProjectSettings()
{

}


void Project_new::Redo()
{
	if (visibleDomain)
		visibleDomain->Redo();
}


void Project_new::RunFullDomain()
{

}


void Project_new::RunSubdomain(QString subdomain)
{

}


void Project_new::SaveProject()
{

}


void Project_new::SelectFullDomainCircleElements()
{
	if (fullDomain)
		fullDomain->UseTool(CircleToolType, ElementSelection);
}


void Project_new::SelectFullDomainClickElements()
{
	if (fullDomain)
		fullDomain->UseTool(ClickToolType, ElementSelection);
}


void Project_new::SelectFullDomainPolygonElements()
{
	if (fullDomain)
		fullDomain->UseTool(PolygonToolType, ElementSelection);
}


void Project_new::SelectFullDomainRectangleElements()
{
	if (fullDomain)
		fullDomain->UseTool(RectangleToolType, ElementSelection);
}


void Project_new::Undo()
{
	if (visibleDomain)
		visibleDomain->Undo();
}


void Project_new::ProjectTreeItemChanged(QTreeWidgetItem *item, QTreeWidgetItem *)
{
	Domain_new *selectedDomain = DetermineSelectedDomain(item);
	if (selectedDomain && selectedDomain != visibleDomain)
	{
		SetVisibleDomain(selectedDomain);
	}
}
