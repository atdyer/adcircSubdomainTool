#include "SelectionLayer.h"

SelectionLayer::SelectionLayer()
{
	// No OpenGL stuff created  yet
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;

	glLoaded = false;

	pointShader = 0;
	outlineShader = 0;
	fillShader = 0;
}


SelectionLayer::~SelectionLayer()
{

	// Clean up the OpenGL stuff

	DEBUG("Deleting Selection Layer. Layer ID: " << GetID());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	if (pointShader)
	{
		DEBUG("Deleting Solid Shader. Shader ID: " << pointShader->GetID());
		delete pointShader;
	}
	if (outlineShader)
	{
		DEBUG("Deleting Solid Shader. Shader ID: " << outlineShader->GetID());
		delete outlineShader;
	}
	if (fillShader)
	{
		DEBUG("Deleting Solid Shader. Shader ID: " << fillShader->GetID());
		delete fillShader;
	}


	// Clean up the Undo/Redo stuff
	for (unsigned int i=0; i<undoStack.size(); i++)
	{
		Action *curr = undoStack.top();
		undoStack.pop();
		if (curr)
			delete curr;
	}
	for (unsigned int i=0; i<redoStack.size(); i++)
	{
		Action *curr = redoStack.top();
		redoStack.pop();
		if (curr)
			delete curr;
	}
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
			if (pointShader->Use())
				glDrawArrays(GL_POINTS, 0, selectedNodes.size());

		}
	}
}


void SelectionLayer::InitializeGL()
{
	// Create new shaders
	pointShader = new SolidShader();
	outlineShader = new SolidShader();
	fillShader = new SolidShader();

	// Set shader properties
	pointShader->SetColor(0.0, 0.0, 0.0, 0.7);
	outlineShader->SetColor(0.2, 0.2, 0.2, 0.2);
	fillShader->SetColor(0.4, 0.4, 0.4, 0.2);
	if (camera)
	{
		pointShader->SetCamera(camera);
		outlineShader->SetCamera(camera);
		fillShader->SetCamera(camera);
	}

	glGenVertexArrays(1, &VAOId);
	glGenBuffers(1, &VBOId);
	glGenBuffers(1, &IBOId);
	glBindVertexArray(VAOId);

	// Set up the VBO attribute pointer
	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

	glBindVertexArray(0);

	GLenum errorCheck = glGetError();
	if (errorCheck == GL_NO_ERROR)
	{
		if (VAOId && VBOId && IBOId)
		{
			DEBUG("Selection Layer Data Loaded");
			glLoaded = true;
		} else {
			DEBUG("Selection Layer Data Not Loaded");
		}
	} else {
		const GLubyte *errString = gluErrorString(errorCheck);
		DEBUG("SelectionLayer OpenGL Error: " << errString);
		glLoaded = false;
	}

	glLoaded = true;
}


void SelectionLayer::UpdateVertexBuffer()
{
	if (!glLoaded)
		InitializeGL();

	const size_t VertexBufferSize = 4*sizeof(GLfloat)*selectedNodes.size();
	if (VertexBufferSize && VAOId && VBOId)
	{
		glBindVertexArray(VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
		GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			Node* currNode;
			int i=0;
			for (std::map<unsigned int, Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); it++, i++)
			{
				currNode = it->second;
				glNodeData[4*i+0] = (GLfloat)currNode->normX;
				glNodeData[4*i+1] = (GLfloat)currNode->normY;
				glNodeData[4*i+2] = (GLfloat)currNode->normZ;
				glNodeData[4*i+3] = (GLfloat)1.0;

			}
		} else {
			glLoaded = false;
			DEBUG("ERROR: Mapping vertex buffer for SelectionLayer " << GetID());
			emit emitMessage("<p style:color='red'><strong>Error: Unable to load vertex data to GPU (Selection Layer)</strong>");
			return;
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			DEBUG("ERROR: Unmapping vertex buffer for SelectionLayer " << GetID());
			return;
		}
	}

	GLenum errorCheck = glGetError();
	if (errorCheck == GL_NO_ERROR)
	{
		if (VAOId && VBOId && IBOId)
		{
			DEBUG("Selection Layer Data Loaded");
			glLoaded = true;
		} else {
			DEBUG("Selection Layer Data Not Loaded");
		}
	} else {
		const GLubyte *errString = gluErrorString(errorCheck);
		DEBUG("SelectionLayer OpenGL Error: " << errString);
		glLoaded = false;
	}

	emit refreshed();
}


void SelectionLayer::LoadDataToGPU()
{

//	const size_t IndexBufferSize = 4*sizeof(GLfloat)*selectedElements.size();

//	// Send Index Data
//	if (IndexBufferSize)
//	{
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
//		GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
//		if (glElementData)
//		{
//			Element* currElement;
//			int i=0;
//			for (std::map<unsigned int, Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); it++, i++)
//			{
//				currElement = it->second;
//				glElementData[3*i+0] = (GLuint)currElement->n1-1;
//				glElementData[3*i+1] = (GLuint)currElement->n2-1;
//				glElementData[3*i+2] = (GLuint)currElement->n3-1;
//			}
//		} else {
//			glLoaded = false;
//			DEBUG("ERROR: Mapping index buffer for SelectionLayer " << GetID());
//			emit emitMessage("<p style:color='red'><strong>Error: Unable to load index data to GPU (Selection Layer)</strong>");
//			return;
//		}

//		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
//		{
//			glLoaded = false;
//			DEBUG("ERROR: Unmapping index buffer for SelectionLayer " << GetID());
//			return;
//		}
//	}
}


void SelectionLayer::SetCamera(GLCamera *cam)
{
	camera = cam;
	if (pointShader)
		pointShader->SetCamera(cam);
	if (outlineShader)
		outlineShader->SetCamera(cam);
	if (fillShader)
		fillShader->SetCamera(cam);
}


void SelectionLayer::SelectNode(Node *node)
{
	// Add the node to list of selected nodes

	// Update the GPU data
//	UpdateDataOnGPU();
}


void SelectionLayer::SelectNodes(std::vector<Node *> nodes)
{
	if (nodes.size() > 0)
	{
		if (selectedNodes.size() == 0)
		{
			for (unsigned int i=0; i<nodes.size(); i++)
			{
				selectedNodes[nodes[i]->nodeNumber] = nodes[i];
			}
			NodeAction *newAction = new NodeAction(selectedNodes);
			newAction->SetSelectionLayer(this);
			undoStack.push(newAction);
			emit undoAvailable(true);
		} else {

			// Don't want to double-select nodes
			std::map<unsigned int, Node*> actualSelection;
			for (unsigned int i=0; i<nodes.size(); i++)
			{
				if (selectedNodes.count(nodes[i]->nodeNumber) == 0)
				{
					selectedNodes[nodes[i]->nodeNumber] = nodes[i];
					actualSelection[nodes[i]->nodeNumber] = nodes[i];
				}
			}

			if (actualSelection.size() > 0)
			{
				NodeAction *newAction = new NodeAction(actualSelection);
				newAction->SetSelectionLayer(this);
				undoStack.push(newAction);
				emit undoAvailable(true);
			}

		}

		// Clear the Redo stack
		for (unsigned int i=0; i<redoStack.size(); i++)
		{
			Action *curr = redoStack.top();
			redoStack.pop();
			if (curr)
				delete curr;
		}
		redoAvailable(false);

		emit numNodesSelected(selectedNodes.size());
		UpdateVertexBuffer();
	}
}


/**
 * @brief Function used by actions to perform selection of a large number of Nodes
 * @param nodes
 */
void SelectionLayer::SelectNodes(std::map<unsigned int, Node *> nodes)
{
	selectedNodes.insert(nodes.begin(), nodes.end());
	UpdateVertexBuffer();
}


/**
 * @brief Function used by actions to perform deselection of a large number of nodes
 * @param nodes
 */
void SelectionLayer::DeselectNodes(std::map<unsigned int, Node *> nodes)
{
	for (std::map<unsigned int, Node*>::iterator it=nodes.begin(); it != nodes.end(); it++)
	{
		selectedNodes.erase(it->first);
	}
	UpdateVertexBuffer();
}


void SelectionLayer::undo()
{
	// Undo the last selection/deselection
	if (undoStack.size() > 0)
	{
		Action *undoAction = undoStack.top();
		undoStack.pop();
		if (undoAction)
		{
			undoAction->UndoAction();
			redoStack.push(undoAction);
			emit redoAvailable(true);
		}
	}

	if (undoStack.size() == 0)
		emit undoAvailable(false);

}


void SelectionLayer::redo()
{
	if (redoStack.size() > 0)
	{
		Action *redoAction = redoStack.top();
		redoStack.pop();
		if (redoAction)
		{
			redoAction->RedoAction();
			undoStack.push(redoAction);
			emit undoAvailable(true);
		}
	}

	if (redoStack.size() == 0)
		emit redoAvailable(false);
}
