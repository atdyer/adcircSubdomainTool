#include "CircleTool.h"

CircleTool::CircleTool()
{
	x = 0.0;
	domainX = 0.0;
	y = 0.0;
	domainY = 0.0;
	rad = 0.0;

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


void CircleTool::Draw()
{
	glLoadIdentity();
	gluOrtho2D(l, r, b, t);
	glTranslatef(l+2*r*x/w, t+2*b*y/h, 0.0);
	glColor4f(0.0, 0.0, 0.0, 0.5);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	gluDisk(quad, rad, 5.0, 100, 2);
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
	x = newX;
	y = newY;

	emit CircleStatsSet(x, y, rad);
}


void CircleTool::SetDomainCenter(float newX, float newY)
{
	domainX = newX;
	domainY = newY;
}


void CircleTool::SetRadius(float newRad)
{
	rad = 2*newRad/h;
}


float CircleTool::GetX()
{
	return x;
}


float CircleTool::GetDomainX()
{
	return domainX;
}


float CircleTool::GetY()
{
	return y;
}


float CircleTool::GetDomainY()
{
	return domainY;
}


float CircleTool::GetRadius()
{
	return rad;
}
