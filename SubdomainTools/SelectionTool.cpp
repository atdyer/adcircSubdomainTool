#include "SelectionTool.h"

SelectionTool::SelectionTool()
{

}


void SelectionTool::UseTool(SelectionType newMode)
{
	SetSelectionMode(newMode);
	UseTool();
}


//SelectionTool::~SelectionTool()
//{

//}


//void SelectionTool::SetCamera(GLCamera *cam)
//{
//	camera = cam;
//	UpdateCamera();
//}


//void SelectionTool::SetTerrainLayer(TerrainLayer *layer)
//{
//	terrain = layer;
//	UpdateTerrain();
//}


//void SelectionTool::SetViewportSize(float w, float h)
//{
//	this->w = w;
//	this->h = h;
//	this->l = -1.0*w/h;
//	this->r = 1.0*w/h;
//	this->b = -1.0;
//	this->t = 1.0;
//}
