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
	}

//	DEBUG("Translate to: " << xNormal << ", " << yNormal);
//	DEBUG("Translate to: " << l+2*r*xPixel/w << ", " << t+2*b*yPixel/h);
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

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


void CircleTool::SetRadiusPoint(float x, float y)
{
	radPixel = sqrt(pow(xPixel - x, 2.0) + pow(yPixel - y, 2.0));
//	radDomain = 2*radPixel/h;
//	radNormal = radDomain;

//	DEBUG("rad-pixel: " << radPixel << "\trad-domain: " << radDomain);

	emit CircleStatsSet(xDomain, yDomain, 2*radPixel/h);
}


void CircleTool::ScaleCircle(float scale)
{
	if (scale > 0)
		radPixel *= zoomScale;
	else
		radPixel /= zoomScale;

	if (camera)
	{
		float newX, newY;
		camera->GetProjectedPoint(xNormal, yNormal, &newX, &newY);
//		DEBUG("x: " << xNormal << " -> " << newX<< "\ty: " << yNormal << " -> " << newY);
//		DEBUG("x-pixel: " << xPixel << "\ty-pixel: " << yPixel);

//		xPixel = newX;
//		yPixel = newY;
	}
}


float CircleTool::GetX()
{
	return xPixel;
}


float CircleTool::GetDomainX()
{
	return xDomain;
}


float CircleTool::GetY()
{
	return yPixel;
}


float CircleTool::GetDomainY()
{
	return yDomain;
}


float CircleTool::GetRadius()
{
	return radPixel;
}


void CircleTool::CircleFinished()
{

	if (terrain)
	{
		DEBUG("x: " << xDomain << "\ty: " << yDomain << "\trad: " << 2*radPixel/h);
		std::vector<Node*> nodes = terrain->GetNodesFromCircle(-77.771, 34.001, 0.2);
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
