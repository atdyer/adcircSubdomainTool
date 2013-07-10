#include "SelectionLayer.h"

SelectionLayer::SelectionLayer()
{
	// No OpenGL stuff created  yet
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;

	// Create new shaders
	pointShader = new SolidShader();
	outlineShader = new SolidShader();
	fillShader = new SolidShader();

	// Set shader properties
	pointShader->SetColor(0.2, 0.2, 0.2, 0.5);
	outlineShader->SetColor(0.2, 0.2, 0.2, 0.2);
	fillShader->SetColor(0.4, 0.4, 0.4, 0.2);

	undoable = false;
	glLoaded = false;
}


SelectionLayer::~SelectionLayer()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	if (pointShader)
		delete pointShader;
	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
}


void SelectionLayer::Draw()
{
	if (glLoaded)
	{
		glBindVertexArray(VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				glDrawElements(GL_TRIANGLES, selectedElements.size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				glDrawElements(GL_TRIANGLES, selectedElements.size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (pointShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			// Draw points here using glDrawArrays()
		}
	}
}


void SelectionLayer::LoadDataToGPU()
{

}


void SelectionLayer::SetCamera(GLCamera *cam)
{
	if (pointShader)
		pointShader->SetCamera(cam);
	if (outlineShader)
		outlineShader->SetCamera(cam);
	if (fillShader)
		fillShader->SetCamera(cam);
}


void SelectionLayer::SelectNode(Node *node)
{

}


void SelectionLayer::SelectNodes(std::vector<Node *> nodes)
{

}


void SelectionLayer::undo()
{

}
