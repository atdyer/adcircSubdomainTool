#include "CircleTool.h"

CircleTool::CircleTool()
{
	x = 0.0;
	y = 0.0;
	rad = 0.0;

	w = 800;
	h = 600;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;

	quad = gluNewQuadric();
	gluQuadricNormals(quad, GLU_SMOOTH);

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
	glColor3f(0.0, 0.0, 0.0);
	gluDisk(quad, rad, rad, 100, 1);
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
}


void CircleTool::SetRadius(float newRad)
{
	rad = 2*newRad/h;
}


float CircleTool::GetX()
{
	return x;
}


float CircleTool::GetY()
{
	return y;
}


float CircleTool::GetRadius()
{
	return rad;
}
