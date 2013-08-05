#include "PolygonTool.h"

PolygonTool::PolygonTool()
{
	terrain = 0;
	camera = 0;

	glLoaded = false;
	visible = false;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	lineShader = 0;

	mouseX = 0.0;
	mouseY = 0.0;
	mousePressed = false;
	mouseMoved = false;

	SetSelectionMode(ElementSelection);

	pointCount = 0;

	w = 800;
	h = 600;
	l = -1.0;
	r = 1.0;
	b = -1.0;
	t = 1.0;
}

PolygonTool::~PolygonTool()
{
	/* Clean up shader */
	if (lineShader)
		delete lineShader;

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


void PolygonTool::Draw()
{
	if (visible && glLoaded && pointCount)
	{
		glBindVertexArray(VAOId);
		if (lineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glLineWidth(3.0);
			if (lineShader->Use())
				glDrawElements(GL_LINE_LOOP, pointCount+1, GL_UNSIGNED_INT, (GLvoid*)0);
			glLineWidth(1.0);
		}
		glBindVertexArray(0);
		glUseProgram(0);
	}
}


void PolygonTool::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (lineShader)
		lineShader->SetCamera(camera);
}


void PolygonTool::SetTerrainLayer(TerrainLayer *layer)
{
	terrain = layer;
}


void PolygonTool::SetViewportSize(float w, float h)
{
	this->w = w;
	this->h = h;
	this->l = -1.0*w/h;
	this->r = 1.0*w/h;
	this->b = -1.0;
	this->t = 1.0;
}


void PolygonTool::SetSelectionMode(SelectionType newMode)
{
	selectionMode = newMode;
}


void PolygonTool::MouseClick(int, int)
{
	mousePressed = true;
	mouseMoved = false;
}


void PolygonTool::MouseMove(int x, int y)
{
	mouseMoved = true;
	if (!mousePressed && camera)
	{
		camera->GetUnprojectedPoint(x, y, &mouseX, &mouseY);
		if (pointCount > 0)
			UpdateMouseVertex();
	}
}


void PolygonTool::MouseRelease(int x, int y)
{
	mousePressed = false;
	if (!mouseMoved && camera)
	{
		camera->GetUnprojectedPoint(x, y, &mouseX, &mouseY);
		if (CheckForDoubleClick(mouseX, mouseY))
		{
			FinishDrawingTool();
		} else {
			AddPoint(mouseX, mouseY);
		}
	}
}


void PolygonTool::MouseWheel(QWheelEvent *event)
{

}


void PolygonTool::KeyPress(QKeyEvent *event)
{
	if (event->key() == Qt::Key_Enter)
	{
		emit ToolFinishedDrawing();
	}
}


std::vector<Element*> PolygonTool::GetSelectedElements()
{
	return selectedElements;
}


void PolygonTool::StartUsingTool()
{
	if (!glLoaded)
		InitializeGL();
	visible = true;
}


void PolygonTool::FinishDrawingTool()
{
	visible = false;
	pointsList.clear();
	pointCount = 0;
	emit ToolFinishedDrawing();
}


void PolygonTool::InitializeGL()
{
	if (!glLoaded)
	{
		if (!lineShader)
			lineShader = new SolidShader();
		lineShader->SetColor(0.0, 0.0, 0.0, 0.5);
		lineShader->SetCamera(camera);

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

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Polygon Tool Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Polygon Tool OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


void PolygonTool::UpdateVertexBuffer()
{
	if (glLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*(pointCount+1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_DYNAMIC_DRAW);
		GLfloat* glNodeData = (GLfloat*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			for (unsigned int i=0; i<pointCount; ++i)
			{
				glNodeData[4*i+0] = (GLfloat)pointsList[i].x;
				glNodeData[4*i+1] = (GLfloat)pointsList[i].y;
				glNodeData[4*i+2] = (GLfloat)1.0;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
			glNodeData[4*pointCount+0] = (GLfloat)mouseX;
			glNodeData[4*pointCount+1] = (GLfloat)mouseY;
			glNodeData[4*pointCount+2] = (GLfloat)1.0;
			glNodeData[4*pointCount+3] = (GLfloat)1.0;
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
	}
}


void PolygonTool::UpdateIndexBuffer()
{
	if (glLoaded)
	{
		const size_t IndexBufferSize = sizeof(GLuint)*(pointCount+1);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glElementData)
		{
			for (int i=0; i<pointCount+1; ++i)
			{
				glElementData[i] = i;
			}
		}
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	}
}


void PolygonTool::UpdateMouseVertex()
{
	if (glLoaded)
	{
		const size_t OffsetSize = 4*sizeof(GLfloat)*pointCount;
		const size_t ReplacementSize = 2*sizeof(GLfloat);
		GLfloat Replacement[2] = {(GLfloat)mouseX, (GLfloat)mouseY};

		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferSubData(GL_ARRAY_BUFFER, OffsetSize, ReplacementSize, &Replacement[0]);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
}


void PolygonTool::AddPoint(float x, float y)
{
	pointsList.push_back(Point(x, y));
	++pointCount;
	UpdateVertexBuffer();
	UpdateIndexBuffer();
}


bool PolygonTool::CheckForDoubleClick(float x, float y)
{
	if (pointsList.size() > 0 && pointsList[pointsList.size()-1].x == x && pointsList[pointsList.size()-1].y == y)
		return true;
	return false;
}
