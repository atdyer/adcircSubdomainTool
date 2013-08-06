#include "CircleTool.h"


/**
 * @brief Constructor that initializes the tool with default values
 */
CircleTool::CircleTool()
{
	terrain = 0;
	camera = 0;

	selectionMode = ElementSelection;

	visible = false;
	mousePressed = false;

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
 * This function draws the circle using the gluDisk function.
 *
 */
void CircleTool::Draw()
{
	if (visible)
	{
		glUseProgram(0); // Turn off any shaders that were previously being used
		glLoadIdentity();
		gluOrtho2D(l, r, b, t);
		glTranslatef(l+2*r*xPixel/w, t+2*b*yPixel/h, 0.0);
		glColor4f(0.0, 0.0, 0.0, 0.5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		gluDisk(quad, 2*radPixel/h, 5.0, 100, 2);
		glTranslatef(-(l+2*r*xPixel/w), -(t+2*b*yPixel/h), 0.0);
	}
}


/**
 * @brief Set the GLCamera that will be used to draw the circle.
 *
 * @param cam Pointer to the desired GLCamera
 */
void CircleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
}


/**
 * @brief Set the TerrainLayer that Nodes will come from when looking for
 * Nodes inside of the circle
 *
 * @param layer Pointer to the desired TerrainLayer
 */
void CircleTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


/**
 * @brief Sets internal values of the viewport size that are used to draw
 * the circle
 *
 * Sets the internal values of the viewport size that are used to draw the circle.
 * This needs to be called every time the size of the OpenGL context changes size.
 *
 * @param w The viewport width in pixels
 * @param h The viewport height in pixels
 */
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


/**
 * @brief Sets the selection mode currently being used by the circle tool
 *
 * Sets the selection mode currently being used by the circle tool. The mode
 * determines what is selected when the user draws a circle using the tool.
 *
 * @param newMode The new mode
 */
void CircleTool::SetSelectionMode(SelectionType newMode)
{
	selectionMode = newMode;
}


void CircleTool::MouseClick(QMouseEvent *event)
{
	visible = true;
	mousePressed = true;
	SetCenter(event->x(), event->y());
	emit Instructions(QString("Drag to resize circle, drop to select elements"));
}


void CircleTool::MouseMove(QMouseEvent *event)
{
	if (mousePressed)
	{
		SetRadiusPoint(event->x(), event->y());
	}
}


void CircleTool::MouseRelease(QMouseEvent*)
{
	mousePressed = false;
	emit Message(QString("Circle Tool:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Center: (<b>")
		     .append(QString::number(xDomain, 'g', 8))
		     .append("</b>, <b>")
		     .append(QString::number(yDomain, 'g', 8))
		     .append("</b>)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Radius: <b>")
		     .append(QString::number(radDomain))
		     .append("</b>"));
	emit ToolFinishedDrawing();
}


void CircleTool::MouseWheel(QWheelEvent*)
{

}


void CircleTool::KeyPress(QKeyEvent*)
{

}


void CircleTool::UseTool()
{
	ResetTool();
	emit Instructions(QString("Click to drop circle center"));
}


std::vector<Element*> CircleTool::GetSelectedElements()
{
	FindElements();
	return selectedElements;
}


/**
 * @brief Set the center of the circle
 *
 * Sets the center of the circle. Typically called once when the user clicks to start
 * drawing the circle.
 *
 * @param newX The x-coordinate of the circle center, in pixels
 * @param newY The y-coordiante of the circle center, in pixels
 */
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

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


/**
 * @brief Set the the second point that defines the radius of the circle
 *
 * Sets the point that is used to define the radius of the circle. The distance
 * between this point and the circle center define the radius, which is used
 * to draw that circle around the center point.
 *
 * @param newX The x-coordinate of the circle edge, in pixels
 * @param newY The y-coordinate of the circle edge, in pixels
 */
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

	radPixel = Distance(xPixel, yPixel, edgeXPixel, edgeYPixel);
	radNormal = Distance(xNormal, yNormal, edgeXNormal, edgeYNormal);
	radDomain = Distance(xDomain, yDomain, edgeXDomain, edgeYDomain);

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


void CircleTool::FindElements()
{
	if (terrain)
	{
		selectedElements = terrain->GetElementsFromCircle(xNormal, yNormal, radNormal);
	}
}


void CircleTool::ResetTool()
{
	visible = false;
	xPixel = 0.0;
	xNormal = 0.0;
	xDomain = 0.0;
	yPixel = 0.0;
	yNormal = 0.0;
	yDomain = 0.0;
	radPixel = 0.0;
	radNormal = 0.0;
	radDomain = 0.0;
	selectedNodes.clear();
	selectedElements.clear();
}


/**
 * @brief Helper function that calculates the distance between two points
 *
 * @param x1 The x-coordinate of the first point
 * @param y1 The y-coordinate of the first point
 * @param x2 The x-coordinate of the second point
 * @param y2 The y-coordinate of the second point
 * @return The distance between the two points
 */
float CircleTool::Distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2-x1, 2.0)+pow(y2-y1, 2.0));
}
