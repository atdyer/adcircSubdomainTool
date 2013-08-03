#include "CircleTool.h"


/**
 * @brief Constructor that initializes the tool with default values
 */
CircleTool::CircleTool()
{
	terrain = 0;
	camera = 0;

	selectionMode = ElementSelection;

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
	if (radPixel > 0.0)
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
 * @brief Set the GLCamera that will be used to draw the circle.
 *
 * @param cam Pointer to the desired GLCamera
 */
void CircleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
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

	radPixel = distance(xPixel, yPixel, edgeXPixel, edgeYPixel);
	radNormal = distance(xNormal, yNormal, edgeXNormal, edgeYNormal);
	radDomain = distance(xDomain, yDomain, edgeXDomain, edgeYDomain);

	emit CircleStatsSet(xDomain, yDomain, radDomain);
}


std::vector<Element*> CircleTool::GetSelectedElements()
{
	return selectedElements;
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


/**
 * @brief Tells the circle tool that the user has finished drawing the circle
 *
 * Tells the circle tool that the user has finished drawing the circle and that
 * it is time to find all of the Nodes inside of the circle. Typically called
 * after the user stops clicking to drop the circle.
 *
 * Resets all coordinate and radius values to zero to prepare to draw another circle.
 *
 */
void CircleTool::CircleFinished()
{
	emit ToolFinishedDrawing();
	emit CircleStatsFinished();

	if (terrain)
	{
		if (selectionMode == NodeSelection)
		{
//			std::vector<Node*> nodes = terrain->GetNodesFromCircle(xNormal, yNormal, radNormal);
//			emit NodesSelected(nodes);
		}
		else if (selectionMode == ElementSelection)
		{
			selectedElements = terrain->GetElementsFromCircle(xNormal, yNormal, radNormal);
			emit FinishedSearching();
		}
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


/**
 * @brief Helper function that calculates the distance between two points
 *
 * @param x1 The x-coordinate of the first point
 * @param y1 The y-coordinate of the first point
 * @param x2 The x-coordinate of the second point
 * @param y2 The y-coordinate of the second point
 * @return The distance between the two points
 */
float CircleTool::distance(float x1, float y1, float x2, float y2)
{
	return sqrt(pow(x2-x1, 2.0)+pow(y2-y1, 2.0));
}
