#include "Domain.h"

Domain::Domain()
{
	camera = new GLCamera();

	terrainLayer = 0;

	layerThread = new QThread();
	if (layerThread)
	{
		connect(this, SIGNAL(beingDestroyed()), layerThread, SLOT(quit()));
		connect(layerThread, SIGNAL(finished()), layerThread, SLOT(deleteLater()));
		layerThread->start();
	}

	progressBar = 0;
	loadingLayer = 0;
}


Domain::~Domain()
{
	if (terrainLayer)
		delete terrainLayer;
}


void Domain::Draw()
{
	if (terrainLayer)
		terrainLayer->Draw();
}


void Domain::Zoom(float zoomAmount)
{
	if (camera)
		camera->Zoom(zoomAmount);
}


void Domain::Pan(float dx, float dy)
{
	if (camera)
		camera->Pan(dx, dy);
}


void Domain::SetWindowSize(float l, float r, float b, float t, float n, float f)
{
	if (camera)
		camera->SetWindowSize(l, r, b, t, n, f);

	// Set circle tool viewport size
}


void Domain::SetFort14Location(std::string newLoc)
{
	if (!terrainLayer)
	{
		terrainLayer = new TerrainLayer();
		loadingLayer = terrainLayer;
		terrainLayer->moveToThread(layerThread);

		if (progressBar)
		{
			connect(terrainLayer, SIGNAL(startedReadingData()), progressBar, SLOT(show()));
			connect(terrainLayer, SIGNAL(progress(int)), progressBar, SLOT(setValue(int)));
			connect(terrainLayer, SIGNAL(finishedReadingData()), progressBar, SLOT(hide()));
		}

		connect(terrainLayer, SIGNAL(emitMessage(QString)), this, SIGNAL(emitMessage(QString)));
		connect(terrainLayer, SIGNAL(finishedReadingData()), this, SLOT(LoadLayerToGPU()));
		connect(terrainLayer, SIGNAL(finishedLoadingToGPU()), this, SIGNAL(updateGL()));

		terrainLayer->SetCamera(camera);
		terrainLayer->UseSolidOutlineShader(0.2, 0.2, 0.2, 0.1);
		terrainLayer->UseSolidFillShader(0.1, 0.8, 0.1, 1.0);
	}
	terrainLayer->SetFort14Location(newLoc);
}


void Domain::SetFort15Location(std::string newLoc)
{

}


void Domain::SetFort63Location(std::string newLoc)
{

}


void Domain::SetFort64Location(std::string newLoc)
{

}


void Domain::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;
}


std::string Domain::GetFort14Location()
{
	if (terrainLayer)
		return terrainLayer->GetFort14Location();
	return std::string("");
}


std::string Domain::GetFort15Location()
{
	return std::string("");
}


std::string Domain::GetFort63Location()
{
	return std::string("");
}


std::string Domain::GetFort64Location()
{
	return std::string("");
}


SolidShaderProperties Domain::GetTerrainSolidOutline()
{
	if (terrainLayer)
		return terrainLayer->GetSolidOutline();
	return SolidShaderProperties();
}


SolidShaderProperties Domain::GetTerrainSolidFill()
{
	if (terrainLayer)
		return terrainLayer->GetSolidFill();
	return SolidShaderProperties();
}


GradientShaderProperties Domain::GetTerrainGradientOutline()
{
	if (terrainLayer)
		return terrainLayer->GetGradientOutline();
	return GradientShaderProperties();
}


GradientShaderProperties Domain::GetTerrainGradientFill()
{
	if (terrainLayer)
		return terrainLayer->GetGradientFill();
	return GradientShaderProperties();
}


void Domain::LoadLayerToGPU()
{
	if (loadingLayer)
	{
		loadingLayer->LoadDataToGPU();
		disconnect(loadingLayer, SIGNAL(finishedReadingData()), this, SLOT(LoadLayerToGPU()));
		loadingLayer = 0;
	}
}


