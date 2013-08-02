#include "RectangleTool.h"

RectangleTool::RectangleTool()
{
	terrain = 0;
	camera = 0;

	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	fillShader = 0;
	for (int i=0; i<12; i++)
		vertexPoints[i][0] = vertexPoints[i][1] = 0.0;

	selectionMode = elementSelection;

	firstCornerPixel[0] = 0.0;
	firstCornerPixel[1] = 0.0;
	secondCornerPixel[0] = 0.0;
	secondCornerPixel[1] = 0.0;
	firstCornerNormal[0] = 0.0;
	firstCornerNormal[1] = 0.0;
	secondCornerNormal[0] = 0.0;
	secondCornerNormal[1] = 0.0;
	firstCornerDomain[0] = 0.0;
	firstCornerDomain[1] = 0.0;
	secondCornerDomain[0] = 0.0;
	secondCornerDomain[1] = 0.0;

	w = 800;
	h = 600;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;
}


RectangleTool::~RectangleTool()
{
	/* Clean up shader */
	if (fillShader)
		delete fillShader;

	/* Clean up OpenGL stuff */
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);
}


void RectangleTool::Draw()
{

}


void RectangleTool::InitializeGL()
{

}


void RectangleTool::UpdateGL()
{

}


void RectangleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
}


void RectangleTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
}


void RectangleTool::SetFirstCorner(int newX, int newY)
{
	firstCornerPixel[0] = newX;
	firstCornerPixel[1] = newY;

	if (camera)
		camera->GetUnprojectedPoint(firstCornerPixel[0], firstCornerPixel[1], &firstCornerNormal[0], &firstCornerNormal[1]);
	else
		DEBUG("Rectangle Tool: No Camera");

	if (terrain)
	{
		firstCornerDomain[0] = terrain->GetUnprojectedX(firstCornerNormal[0]);
		firstCornerDomain[1] = terrain->GetUnprojectedY(firstCornerNormal[1]);
	} else {
		DEBUG("Rectangle Tool: No Terrain");
	}
}


void RectangleTool::SetSecondCorner(int newX, int newY)
{
	secondCornerPixel[0] = newX;
	secondCornerPixel[1] = newY;

	if (camera)
		camera->GetUnprojectedPoint(secondCornerPixel[0], secondCornerPixel[1], &secondCornerNormal[0], &secondCornerNormal[1]);
	else
		DEBUG("Rectangle Tool: No Camera");

	if (terrain)
	{
		secondCornerDomain[0] = terrain->GetUnprojectedX(secondCornerNormal[0]);
		secondCornerDomain[1] = terrain->GetUnprojectedY(secondCornerNormal[1]);
	} else {
		DEBUG("Rectangle Tool: No Terrain");
	}
}


void RectangleTool::RectangleFinished()
{

}


void RectangleTool::CalculateVertexPoints()
{
	float topLeft[2] = {0.0, 0.0};
	float bottomRight[2] = {0.0, 0.0};

	/* First, figure out where the second point is in relation to the first */
	if (firstCornerNormal[0] < secondCornerNormal[0])
	{
		/* The first point is to the left of the second point */
		if (firstCornerNormal[1] < secondCornerNormal[1])
		{
			/* The first point is below the second point, we're drawing a first quadrant rectangle */
			topLeft[0] = firstCornerNormal[0];
			topLeft[1] = secondCornerNormal[1];
			bottomRight[0] = secondCornerNormal[0];
			bottomRight[1] = firstCornerNormal[0];
		} else {
			/* The first point is above the second point, we're drawing a fourth quadtrant rectangle */
			topLeft[0] = firstCornerNormal[0];
			topLeft[1] = firstCornerNormal[1];
			bottomRight[0] = secondCornerNormal[0];
			bottomRight[1] = secondCornerNormal[0];
		}
	} else {
		/* The first point is to the right of the second point */
		if (firstCornerNormal[1] < secondCornerNormal[1])
		{
			/* The first point is below the second point, we're drawing a second quadrant rectangle */
			topLeft[0] = secondCornerNormal[0];
			topLeft[1] = secondCornerNormal[1];
			bottomRight[0] = firstCornerNormal[0];
			bottomRight[1] = firstCornerNormal[0];
		} else {
			/* The first point is above the second point, we're drawing a third quadrant rectangle */
			topLeft[0] = secondCornerNormal[0];
			topLeft[1] = firstCornerNormal[1];
			bottomRight[0] = firstCornerNormal[0];
			bottomRight[1] = secondCornerNormal[0];
		}
	}

	/* Set all x-values */
	vertexPoints[0][0] = vertexPoints[1][0] = topLeft[0];
	vertexPoints[2][0] = vertexPoints[3][0] = bottomRight[0];
	vertexPoints[4][0] = vertexPoints[5][0] = vertexPoints[6][0] = vertexPoints[7][0] = -100.0;
	vertexPoints[8][0] = vertexPoints[9][0] = vertexPoints[10][0] = vertexPoints[11][0] = 100.0;

	/* Set all y-values */
	vertexPoints[4][1] = vertexPoints[8][1] = -100.0;
	vertexPoints[7][1] = vertexPoints[11][1] = 100.0;
	vertexPoints[6][1] = vertexPoints[1][1] = vertexPoints[3][1] = vertexPoints[10][1] = topLeft[1];
	vertexPoints[5][1] = vertexPoints[0][1] = vertexPoints[2][1] = vertexPoints[9][1] = bottomRight[1];

}


void RectangleTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


void RectangleTool::SetSelectionMode(SelectionType newMode)
{
	selectionMode = newMode;
}
