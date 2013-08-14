#include "ClickTool.h"

ClickTool::ClickTool()
{
	terrain = 0;
	camera = 0;

	glLoaded = false;
	visible = false;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	pointShader = 0;
	glPoint[0] = glPoint[1] = glPoint[2] =  0.0;
	glPoint[3] = 1.0;
	glIndex = 0;

	mousePressed = false;
	xPixel = 0;
	yPixel = 0;
	xGL = 0.0;
	yGL = 0.0;
}


ClickTool::~ClickTool()
{
	/* Clean up shader */
	if (pointShader)
		delete pointShader;

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


void ClickTool::Draw()
{
	if (glLoaded && visible)
	{
		glBindVertexArray(VAOId);
		if (pointShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			glPointSize(10.0);
			if (pointShader->Use())
				glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, (GLvoid*)0);
			glPointSize(1.0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void ClickTool::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (pointShader)
		pointShader->SetCamera(cam);
}


void ClickTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


void ClickTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
}


void ClickTool::MouseClick(QMouseEvent *event)
{
	mousePressed = true;
	visible = true;
	xPixel = event->x();
	yPixel = event->y();

	UpdateCoordinates();

	if (glLoaded)
		UpdateGL();
}


void ClickTool::MouseMove(QMouseEvent *)
{

}


void ClickTool::MouseRelease(QMouseEvent *event)
{
	mousePressed = false;
	visible = false;
	emit ToolFinishedDrawing();
}


void ClickTool::MouseWheel(QWheelEvent *)
{

}


void ClickTool::KeyPress(QKeyEvent *)
{

}


void ClickTool::UseTool()
{
	if (!glLoaded)
		InitializeGL();
}


std::vector<Node*> ClickTool::GetSelectedNodes()
{
	return selectedNodes;
}


std::vector<Element*> ClickTool::GetSelectedElements()
{
	FindElement();
	return selectedElements;
}


void ClickTool::InitializeGL()
{
	if (!glLoaded)
	{
		if (!pointShader)
			pointShader = new SolidShader();
		pointShader->SetColor(0.0, 0.0, 0.0, 1.0);
		pointShader->SetCamera(camera);

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

		const size_t VertexBufferSize = 4*sizeof(GLfloat);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, &glPoint[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

		const size_t IndexBufferSize = 1*sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, &glIndex, GL_STATIC_DRAW);

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


void ClickTool::UpdateCoordinates()
{
	if (camera)
	{
		camera->GetUnprojectedPoint(float(xPixel), float(yPixel), &xGL, &yGL);
		glPoint[0] = xGL;
		glPoint[1] = yGL;
		DEBUG("Click: " << xGL << ", " << yGL);
	} else {
		xGL = xPixel;
		yGL = yPixel;
	}
}


void ClickTool::UpdateGL()
{
	if (!glLoaded)
		InitializeGL();

	if (glLoaded)
	{
		if (VBOId)
		{
			glBindBuffer(GL_ARRAY_BUFFER, VBOId);
			glBufferSubData(GL_ARRAY_BUFFER, 0, 2*sizeof(GLfloat), &glPoint[0]);
		}
	}
}


void ClickTool::FindElement()
{
	if (terrain)
	{
		selectedElements.clear();
		Element* selectedElement = terrain->GetElement(xGL, yGL);
		if (selectedElement)
			selectedElements.push_back(selectedElement);
	}
}
