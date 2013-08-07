#include "RectangleTool.h"

RectangleTool::RectangleTool()
{
	terrain = 0;
	camera = 0;

	glLoaded = false;
	visible = false;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	fillShader = 0;
	for (int i=0; i<12; i++)
	{
		vertexPoints[i][0] = vertexPoints[i][1] = vertexPoints[i][2] = 0.0;
		vertexPoints[i][3] = 1.0;
	}
	vertexPoints[4][0] = vertexPoints[5][0] = vertexPoints[6][0] = vertexPoints[7][0] = -100.0;
	vertexPoints[8][0] = vertexPoints[9][0] = vertexPoints[10][0] = vertexPoints[11][0] = 100.0;
	vertexPoints[4][1] = vertexPoints[8][1] = -100.0;
	vertexPoints[7][1] = vertexPoints[11][1] = 100.0;
	vertexBufferSize = 12*4*sizeof(GLuint);

	clicking = false;

	indexArray[0][0] = 4;
	indexArray[0][1] = 8;
	indexArray[0][2] = 9;
	indexArray[1][0] = 4;
	indexArray[1][1] = 5;
	indexArray[1][2] = 9;
	indexArray[2][0] = 0;
	indexArray[2][1] = 1;
	indexArray[2][2] = 5;
	indexArray[3][0] = 1;
	indexArray[3][1] = 5;
	indexArray[3][2] = 6;
	indexArray[4][0] = 6;
	indexArray[4][1] = 10;
	indexArray[4][2] = 11;
	indexArray[5][0] = 6;
	indexArray[5][1] = 7;
	indexArray[5][2] = 11;
	indexArray[6][0] = 2;
	indexArray[6][1] = 3;
	indexArray[6][2] = 10;
	indexArray[7][0] = 2;
	indexArray[7][1] = 9;
	indexArray[7][2] = 10;

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
	if (visible && glLoaded)
	{
		glBindVertexArray(VAOId);
		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, (GLvoid*)0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void RectangleTool::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (fillShader)
		fillShader->SetCamera(camera);
}


void RectangleTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
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


void RectangleTool::MouseClick(QMouseEvent *event)
{
	clicking = true;
	visible = true;
	SetFirstCorner(event->x(), event->y());
	emit Instructions(QString("Drag to resize the rectangle. Drop to select elements."));
}


void RectangleTool::MouseMove(QMouseEvent *event)
{
	if (clicking)
		SetSecondCorner(event->x(), event->y());
}


void RectangleTool::MouseRelease(QMouseEvent *event)
{
	clicking = false;
	emit Message(QString("Rectangle Tool:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Width: <b>")
		     .append(QString::number(fabs(secondCornerDomain[0] - firstCornerDomain[0]), 'g', 8))
			.append(QString("</b>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Height: <b>"))
			.append(QString::number(fabs(secondCornerDomain[1] - firstCornerDomain[1]), 'g', 8))
			.append("</b>"));
	emit ToolFinishedDrawing();
}


void RectangleTool::MouseWheel(QWheelEvent *event)
{

}


void RectangleTool::KeyPress(QKeyEvent *event)
{

}


void RectangleTool::UseTool()
{
	ResetTool();
	if (!glLoaded)
		InitializeGL();
	emit Instructions(QString("Click to drop the first corner of the rectangle"));
}


std::vector<Node*> RectangleTool::GetSelectedNodes()
{
	return selectedNodes;
}


std::vector<Element*> RectangleTool::GetSelectedElements()
{
	FindElements();
	return selectedElements;
}


void RectangleTool::InitializeGL()
{
	if (!glLoaded)
	{
		if (!fillShader)
			fillShader = new SolidShader();
		fillShader->SetColor(0.0, 0.0, 0.0, 0.5);
		fillShader->SetCamera(camera);

		if (!VAOId)
			glGenVertexArrays(1, &VAOId);
		if (!VBOId)
			glGenBuffers(1, &VBOId);
		if (!IBOId)
			glGenBuffers(1, &IBOId);

		glBindVertexArray(VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

		glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, NULL, GL_DYNAMIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, &vertexPoints[0][0]);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

		const size_t IndexBufferSize = 8*3*sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, IndexBufferSize, &indexArray[0][0]);

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Rectangle Tool Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Rectangle Tool OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


void RectangleTool::UpdateGL()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded)
	{
		if (VBOId)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBOId);
			glBufferSubData(GL_ARRAY_BUFFER, 0, vertexBufferSize, &vertexPoints[0][0]);
		}
	}
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
		emit RectangleStatsSet(fabs(secondCornerDomain[0] - firstCornerDomain[0]), fabs(secondCornerDomain[1] - firstCornerDomain[1]));
	} else {
		DEBUG("Rectangle Tool: No Terrain");
	}

	CalculateVertexPoints();
	UpdateGL();
}


void RectangleTool::FindElements()
{
	if (terrain)
	{
		selectedElements = terrain->GetElementsFromRectangle(vertexPoints[0][0], vertexPoints[3][0], vertexPoints[0][1], vertexPoints[3][1]);
	}
}


void RectangleTool::ResetTool()
{
	visible = false;
	clicking = false;
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
			bottomRight[1] = firstCornerNormal[1];
		} else {
			/* The first point is above the second point, we're drawing a fourth quadtrant rectangle */
			topLeft[0] = firstCornerNormal[0];
			topLeft[1] = firstCornerNormal[1];
			bottomRight[0] = secondCornerNormal[0];
			bottomRight[1] = secondCornerNormal[1];
		}
	} else {
		/* The first point is to the right of the second point */
		if (firstCornerNormal[1] < secondCornerNormal[1])
		{
			/* The first point is below the second point, we're drawing a second quadrant rectangle */
			topLeft[0] = secondCornerNormal[0];
			topLeft[1] = secondCornerNormal[1];
			bottomRight[0] = firstCornerNormal[0];
			bottomRight[1] = firstCornerNormal[1];
		} else {
			/* The first point is above the second point, we're drawing a third quadrant rectangle */
			topLeft[0] = secondCornerNormal[0];
			topLeft[1] = firstCornerNormal[1];
			bottomRight[0] = firstCornerNormal[0];
			bottomRight[1] = secondCornerNormal[1];
		}
	}

	/* Set all x-values */
	vertexPoints[0][0] = vertexPoints[1][0] = topLeft[0];
	vertexPoints[2][0] = vertexPoints[3][0] = bottomRight[0];

	/* Set all y-values */
	vertexPoints[6][1] = vertexPoints[1][1] = vertexPoints[3][1] = vertexPoints[10][1] = topLeft[1];
	vertexPoints[5][1] = vertexPoints[0][1] = vertexPoints[2][1] = vertexPoints[9][1] = bottomRight[1];
}
