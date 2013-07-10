#include "SelectionLayer.h"

SelectionLayer::SelectionLayer()
{
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	pointShader = 0;
	outlineShader = 0;
	fillShader = 0;

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

}


void SelectionLayer::LoadDataToGPU()
{

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
