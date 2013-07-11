#include "CircleTool.h"

CircleTool::CircleTool()
{
	terrain = 0;
	camera = 0;

	xPixel = 0.0;
	xNormal = 0.0;
	xDomain = 0.0;
	yPixel = 0.0;
	yNormal = 0.0;
	yDomain = 0.0;
	edgeXPixel = 0.0;
	edgeYPixel = 0.0;
	edgeXNormal = 0.0;
	edgeYNormal = 0.0;
	edgeXDomain = 0.0;
	edgeYDomain = 0.0;
	radPixel = 0.0;
	radNormal = 0.0;
	radDomain = 0.0;
	zoomScale = 1.1;

	w = 800;
	h = 600;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;

	quad = gluNewQuadric();
	gluQuadricDrawStyle(quad, GLU_FILL);

}


CircleTool::~CircleTool()
{
	if (quad)
		gluDeleteQuadric(quad);
}


/**
 * @brief Draws the circle
 *
 * This function draws the disk using the values of x, y, and rad.
 *
 */
void CircleTool::Draw()
{
	if (radPixel > 0.0)
	{
		glLoadIdentity();
		gluOrtho2D(l, r, b, t);
		glTranslatef(l+2*r*xPixel/w, t+2*b*yPixel/h, 0.0);
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluDisk(quad, 2*radPixel/h, 5.0, 100, 2);
		glTranslatef(-(l+2*r*xPixel/w), -(t+2*b*yPixel/h), 0.0);
	}
}


void CircleTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


void CircleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
}


void CircleTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
}


void CircleTool::SetCenter(int newX, int newY)
{
	xPixel = newX;
	yPixel = newY;

	if (camera)
	{
		camera->GetUnprojectedPoint(xPixel, yPixel, &xNormal, &yNormal);
	} else {
		DEBUG("Circle Tool: No Camera");
	}

	if (terrain)
	{
		xDomain = terrain->GetUnprojectedX(xNormal);
		yDomain = terrain->GetUnprojectedY(yNormal);
	} else {
		DEBUG("Circle Tool: No Terrain");
	}

//	DEBUG("x-pixel: " << xPixel << "\tx-normal: " << xNormal << "\tx-domain: " << xDomain);
//	DEBUG("y-pixel: " << yPixel << "\ty-normal: " << yNormal << "\ty-domain: " << yDomain);

	emit CircleStatsSet(xNormal, yNormal, radNormal);
}


void CircleTool::SetRadiusPoint(int newX, int newY)
{
	edgeXPixel = newX;
	edgeYPixel = newY;

	if (camera)
	{
		camera->GetUnprojectedPoint(edgeXPixel, edgeYPixel, &edgeXNormal, &edgeYNormal);
	}
	if (terrain)
	{
		edgeXDomain = terrain->GetUnprojectedX(edgeXNormal);
		edgeYDomain = terrain->GetUnprojectedY(edgeYNormal);
	}

	radPixel = distance(xPixel, yPixel, edgeXPixel, edgeYPixel);
	radNormal = distance(xNormal, yNormal, edgeXNormal, edgeYNormal);
	radDomain = distance(xDomain, yDomain, edgeXDomain, edgeYDomain);

	emit CircleStatsSet(edgeXNormal, edgeYNormal, radNormal);
}


void CircleTool::CircleFinished()
{

	if (terrain)
	{
		DEBUG("x: " << xNormal << "\ty: " << yNormal << "\trad: " << radNormal);
//		DEBUG("CENTER\tx: " << xNormal << "\ty: " << yNormal);
//		DEBUG("EDGE\tx:" << edgeXNormal << "\ty: " << edgeYNormal);
//		std::vector<Node*> nodes = terrain->GetNodesFromCircle(-77.771, 34.001, 0.1);
		std::vector<Node*> nodes = terrain->GetNodesFromCircle(xNormal, yNormal, radNormal);
		emit NodesSelected(nodes);
		DEBUG("Number of nodes found: " << nodes.size());
	}


	xPixel = 0.0;
	xNormal = 0.0;
	xDomain = 0.0;
	yPixel = 0.0;
	yNormal = 0.0;
	yDomain = 0.0;
	radPixel = 0.0;
	radNormal = 0.0;
	radDomain = 0.0;
}


float CircleTool::distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2-x1, 2.0)+pow(y2-y1, 2.0));
}
