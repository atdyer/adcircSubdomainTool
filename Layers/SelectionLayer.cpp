#include "SelectionLayer.h"


/**
 * @brief Constructor
 *
 * Initializes all variables to default values.
 *
 */
SelectionLayer::SelectionLayer()
{
	terrainLayer = 0;

	// No OpenGL stuff created  yet
	glLoaded = false;
	camera = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;

	mousePressed = false;
}


/**
 * @brief Destructor
 *
 * The subclass of SelectionLayer is reponsible for any data loaded to
 * the graphics card.
 *
 * The SelectionLayer destructor will delete any actions that have been
 * pushed onto the undo or redo stack.
 *
 */
SelectionLayer::~SelectionLayer()
{
	ClearUndoStack();
	ClearRedoStack();
}


/**
 * @brief Returns the number of selected Nodes
 *
 * Returns the number of selected Nodes. If not overridden in a subclass,
 * always returns 0.
 *
 * @return 0
 */
unsigned int SelectionLayer::GetNumNodesSelected()
{
	return 0;
}


/**
 * @brief Returns the number of selected Elements
 *
 * Returns the number of selected Elements. If not overridden in a subclass,
 * always returns 0.
 *
 * @return 0
 */
unsigned int SelectionLayer::GetNumElementsSelected()
{
	return 0;
}


/**
 * @brief Undoes the last Action that was performed
 *
 * Undoes the last Action that was performed by popping it off of the
 * undo stack, calling the UndoAction method of the class, and pushing
 * it onto the redo stack.
 *
 */
void SelectionLayer::Undo()
{
	if (!undoStack.empty())
	{
		/* Get the pointer to the last performed action */
		Action *latestAction = undoStack.top();

		/* Remove that action from the undo stack */
		undoStack.pop();

		/* Undo the action */
		latestAction->UndoAction();

		/* Put that action onto the redo stack */
		redoStack.push(latestAction);

		/* Let everyone know there is a redo available */
		emit RedoAvailable(true);

		/* Check if there are any undo actions available */
		if (undoStack.empty())
			emit UndoAvailable(false);
	}
}


/**
 * @brief Redoes the last Action that was undone
 *
 * Redoes the last Action that was undone by popping it off of the
 * redo stack, calling the RedoAction method of the class, and pushing
 * it onto the undo stack.
 *
 */
void SelectionLayer::Redo()
{
	if (!redoStack.empty())
	{
		/* Get the pointer to the last undone action */
		Action *latestUndo = redoStack.top();

		/* Remove that action from the redo stack */
		redoStack.pop();

		/* Redo the action */
		latestUndo->RedoAction();

		/* Put that action back onto the undo stack */
		undoStack.push(latestUndo);

		/* Let everyone know there is an undo available */
		emit UndoAvailable(true);

		/* Check if there are any redo actions available */
		if (redoStack.empty())
			emit RedoAvailable(false);
	}
}


/**
 * @brief Deletes the entire undo stack
 *
 * Deletes all Actions that are on the undo stack
 *
 */
void SelectionLayer::ClearUndoStack()
{
	while (!undoStack.empty())
	{
		Action* curr = undoStack.top();
		undoStack.pop();
		if (curr)
			delete curr;
	}
	emit UndoAvailable(false);
}


/**
 * @brief Deletes the entire redo stack
 *
 * Deletes all Actions that are on the redo stack
 *
 */
void SelectionLayer::ClearRedoStack()
{
	while(!redoStack.empty())
	{
		Action* curr = redoStack.top();
		redoStack.pop();
		if (curr)
			delete curr;
	}
	emit RedoAvailable(false);
}



/** OLD CODE STARTS HERE */

//SelectionLayer::SelectionLayer()
//{
//	// No OpenGL stuff created  yet
//	VAOId = 0;
//	VBOId = 0;
//	IBOId = 0;

//	glLoaded = false;

//	pointShader = 0;
//	outlineShader = 0;
//	fillShader = 0;
//}


//SelectionLayer::~SelectionLayer()
//{

//	DEBUG("Deleting Selection Layer. Layer ID: " << GetID());

//	// Clean up the OpenGL stuff
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//	if (VBOId)
//		glDeleteBuffers(1, &VBOId);
//	if (VAOId)
//		glDeleteBuffers(1, &VAOId);
//	if (IBOId)
//		glDeleteBuffers(1, &IBOId);

//	if (pointShader)
//	{
//		DEBUG("Deleting Solid Shader. Shader ID: " << pointShader->GetID());
//		delete pointShader;
//	}
//	if (outlineShader)
//	{
//		DEBUG("Deleting Solid Shader. Shader ID: " << outlineShader->GetID());
//		delete outlineShader;
//	}
//	if (fillShader)
//	{
//		DEBUG("Deleting Solid Shader. Shader ID: " << fillShader->GetID());
//		delete fillShader;
//	}


//	// Clean up the Undo/Redo stuff
//	for (unsigned int i=0; i<undoStack.size(); i++)
//	{
//		Action *curr = undoStack.top();
//		undoStack.pop();
//		if (curr)
//			delete curr;
//	}
//	for (unsigned int i=0; i<redoStack.size(); i++)
//	{
//		Action *curr = redoStack.top();
//		redoStack.pop();
//		if (curr)
//			delete curr;
//	}
//}


///**
// * @brief Draws the selected Nodes and Elements
// *
// * This function overrides the Layer::Draw virtual function. It will draw the currently selected
// * Elements followed by the currently selected Nodes if the data has been loaded to the OpenGL
// * context.
// *
// */
//void SelectionLayer::Draw()
//{
//	if (glLoaded)
//	{
//		glBindVertexArray(VAOId);

//		if (fillShader)
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//			if (fillShader->Use())
//				glDrawElements(GL_TRIANGLES, selectedElements.size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
//		}

//		if (outlineShader)
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			if (outlineShader->Use())
//				glDrawElements(GL_TRIANGLES, selectedElements.size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
//		}

////		if (pointShader)
////		{
////			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
////			if (pointShader->Use())
////				glDrawArrays(GL_POINTS, 0, selectedNodes.size());

////		}

//	}
//}


///**
// * @brief Dummy function. Never actually called on the SelectionLayer
// */
//void SelectionLayer::LoadDataToGPU()
//{

//}


///**
// * @brief Sets the camera used during drawing operations
// *
// * Sets the camera being used during drawing operations
// *
// * @param cam Pointer to the GLCamera object
// */
//void SelectionLayer::SetCamera(GLCamera *cam)
//{
//	camera = cam;
//	if (pointShader)
//		pointShader->SetCamera(cam);
//	if (outlineShader)
//		outlineShader->SetCamera(cam);
//	if (fillShader)
//		fillShader->SetCamera(cam);
//}


///**
// * @brief Returns the number of Nodes that are currently selected
// *
// * Returns the number of Nodes that are currently selected
// *
// * @return The number of Nodes that are currently selected
// */
//unsigned int SelectionLayer::GetNumNodesSelected()
//{
//	return selectedNodes.size();
//}


///**
// * @brief Returns the number of Elements that are currently selected
// *
// * Returns the number of Elements that are currently selected
// *
// * @return The number of Elements that are currently selected
// */
//unsigned int SelectionLayer::GetNumElementsSelected()
//{
//	return selectedElements.size();
//}


///**
// * @brief Initializes the Buffer Objects and GLShader objects necessary for drawing the
// * selection layer.
// *
// * This function initializes the Buffer Objects in the OpenGL context as well as the
// * GLShader objects necessary for drawing the selection layer.
// *
// */
//void SelectionLayer::InitializeGL()
//{
//	// Create new shaders
//	pointShader = new SolidShader();
//	outlineShader = new SolidShader();
//	fillShader = new SolidShader();

//	// Set shader properties
//	pointShader->SetColor(0.0, 0.0, 0.0, 0.7);
//	outlineShader->SetColor(0.2, 0.2, 0.2, 0.2);
//	fillShader->SetColor(0.4, 0.4, 0.4, 0.4);
//	if (camera)
//	{
//		pointShader->SetCamera(camera);
//		outlineShader->SetCamera(camera);
//		fillShader->SetCamera(camera);
//	}

//	glGenVertexArrays(1, &VAOId);
//	glGenBuffers(1, &VBOId);
//	glGenBuffers(1, &IBOId);
//	glBindVertexArray(VAOId);

//	// Set up the VBO attribute pointer
//	glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);

//	glBindVertexArray(0);

//	GLenum errorCheck = glGetError();
//	if (errorCheck == GL_NO_ERROR)
//	{
//		if (VAOId && VBOId && IBOId)
//		{
//			DEBUG("Selection Layer Data Loaded");
//			glLoaded = true;
//		} else {
//			DEBUG("Selection Layer Data Not Loaded");
//			glLoaded = false;
//		}
//	} else {
//		const GLubyte *errString = gluErrorString(errorCheck);
//		DEBUG("SelectionLayer OpenGL Error: " << errString);
//		glLoaded = false;
//	}
//}


///**
// * @brief This function updates the vertex data on the OpenGL context.
// *
// * Updates the vertex data on the OpenGL context. Calling the glBufferData function with
// * NULL as the data pointer tells the OpenGL context that any data currently in the
// * Vertex Buffer can be ignored and overwritten. This allows us to quickly replace all
// * of the data in the Vertex Buffer with the updated data.
// *
// */
//void SelectionLayer::UpdateVertexBuffer()
//{
//	if (!glLoaded)
//		InitializeGL();

//	const size_t VertexBufferSize = 4*sizeof(GLfloat)*selectedNodes.size();
//	if (VertexBufferSize && VAOId && VBOId)
//	{
//		glBindVertexArray(VAOId);
//		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
//		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
//		GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
//		if (glNodeData)
//		{
//			Node* currNode;
//			int i=0;
//			for (std::map<unsigned int, Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); it++, i++)
//			{
//				currNode = it->second;
//				glNodeData[4*i+0] = (GLfloat)currNode->normX;
//				glNodeData[4*i+1] = (GLfloat)currNode->normY;
//				glNodeData[4*i+2] = (GLfloat)currNode->normZ;
//				glNodeData[4*i+3] = (GLfloat)1.0;

//			}
//		} else {
//			glLoaded = false;
//			DEBUG("ERROR: Mapping vertex buffer for SelectionLayer " << GetID());
//			emit emitMessage("<p style:color='red'><strong>Error: Unable to load vertex data to GPU (Selection Layer)</strong>");
//			return;
//		}

//		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
//		{
//			glLoaded = false;
//			DEBUG("ERROR: Unmapping vertex buffer for SelectionLayer " << GetID());
//			return;
//		}
//	}

//	GLenum errorCheck = glGetError();
//	if (errorCheck == GL_NO_ERROR)
//	{
//		if (VAOId && VBOId && IBOId)
//		{
//			DEBUG("Selection Layer Data Loaded");
//			glLoaded = true;
//		} else {
//			DEBUG("Selection Layer Data Not Loaded");
//		}
//	} else {
//		const GLubyte *errString = gluErrorString(errorCheck);
//		DEBUG("SelectionLayer OpenGL Error: " << errString);
//		glLoaded = false;
//	}

//	emit refreshed();
//}


//void SelectionLayer::UpdateIndexBuffer()
//{
//	if (!glLoaded)
//		InitializeGL();

//	const size_t IndexBufferSize = 3*sizeof(GLuint)*selectedElements.size();
//	if (IndexBufferSize && VAOId && IBOId)
//	{
//		glBindVertexArray(VAOId);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
//		GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
//		if (glElementData)
//		{
//			Element* currElement;
//			int i=0;
//			for (std::map<unsigned int, Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); it++, i++)
//			{
//				currElement = it->second;
//				glElementData[3*i+0] = (GLuint)indexingMap[currElement->n1->nodeNumber];
//				glElementData[3*i+1] = (GLuint)indexingMap[currElement->n2->nodeNumber];
//				glElementData[3*i+2] = (GLuint)indexingMap[currElement->n3->nodeNumber];
////				DEBUG(indexingMap[currElement->n1->nodeNumber] << " " << indexingMap[currElement->n2->nodeNumber] << " " << indexingMap[currElement->n3->nodeNumber]);
////				glElementData[3*i+0] = (GLuint)currElement->n1->nodeNumber-1;
////				glElementData[3*i+1] = (GLuint)currElement->n2->nodeNumber-1;
////				glElementData[3*i+2] = (GLuint)currElement->n3->nodeNumber-1;
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

//	GLenum errorCheck = glGetError();
//	if (errorCheck == GL_NO_ERROR)
//	{
//		if (VAOId && VBOId && IBOId)
//		{
//			DEBUG("Selection Layer Data Loaded");
//			glLoaded = true;
//		} else {
//			DEBUG("Selection Layer Data Not Loaded");
//		}
//	} else {
//		const GLubyte *errString = gluErrorString(errorCheck);
//		DEBUG("SelectionLayer OpenGL Error: " << errString);
//		glLoaded = false;
//	}

//	emit refreshed();
//}


//void SelectionLayer::UpdateNodeIndexing()
//{
//	indexingMap.clear();

//	Node* currNode;
//	int i=0;
//	for (std::map<unsigned int, Node*>::iterator it=selectedNodes.begin(); it != selectedNodes.end(); it++, i++)
//	{
//		currNode = it->second;
//		indexingMap[currNode->nodeNumber] = i;
//	}
//	UpdateIndexBuffer();
//}


///**
// * @brief Function used by Action objects to perform selection of a large number of Nodes
// *
// * Special function that should only be used by Action objects to select a large number
// * of Node objects. Actions are created with only the Nodes that are actually selected
// * (ie. duplicates are not included), so a unique set of Nodes is guaranteed in each
// * Action. This function is very fast because it does not check for duplicates and uses
// * the std::map insert function.
// *
// * @param nodes The map of Nodes to select
// */
//void SelectionLayer::SelectNodes(std::map<unsigned int, Node *> nodes)
//{
//	selectedNodes.insert(nodes.begin(), nodes.end());
//	UpdateVertexBuffer();
//	UpdateNodeIndexing();
//	emit numNodesSelected(selectedNodes.size());
//}


///**
// * @brief Function used by Action objects to perform deselection of a large number of nodes
// *
// * Special function that should only be used by Action objects to deselect a large number
// * of Node objects. Action are created with only the Nodes that are actually selected
// * (ie. duplicates are not included), so a unique set of Nodes is guaranteed in each
// * Action. This function is very fast because it does not check for duplicates and uses
// * the std::map insert function.
// *
// * @param nodes The map of Nodes to deselect
// */
//void SelectionLayer::DeselectNodes(std::map<unsigned int, Node *> nodes)
//{
//	for (std::map<unsigned int, Node*>::iterator it=nodes.begin(); it != nodes.end(); it++)
//	{
//		selectedNodes.erase(it->first);
//	}
//	UpdateVertexBuffer();
//	UpdateNodeIndexing();
//	emit numNodesSelected(selectedNodes.size());
//}


//void SelectionLayer::SelectElements(std::map<unsigned int, Element *> elements)
//{
//	selectedElements.insert(elements.begin(), elements.end());
//	UpdateIndexBuffer();
//	emit numElementsSelected(selectedElements.size());
//}


//void SelectionLayer::DeselectElements(std::map<unsigned int, Element *> elements)
//{
//	for (std::map<unsigned int, Element*>::iterator it=elements.begin(); it != elements.end(); it++)
//	{
//		selectedElements.erase(it->first);
//	}
//	UpdateIndexBuffer();
//	emit numElementsSelected(selectedElements.size());
//}


///**
// * @brief Selects/Deselects a single Node
// *
// * Selects a single Node if that Node is not already selected. If it is
// * already selected, the Node is deselected
// *
// * @param node Pointer to the Node to select/deselect
// */
//void SelectionLayer::SelectNode(Node *node)
//{
//	// Add the node to list of selected nodes

//	// Update the GPU data
////	UpdateDataOnGPU();
//}


///**
// * @brief Selects all Nodes in the provided list of Nodes
// *
// * Selects every Node in the provided list that is not already selected
// *
// * @param nodes The list of nodes to select. Can include Nodes that are already selected.
// */
//void SelectionLayer::SelectNodes(std::vector<Node *> nodes)
//{
//	if (nodes.size() > 0)
//	{
//		if (selectedNodes.size() == 0)
//		{
//			for (unsigned int i=0; i<nodes.size(); i++)
//			{
//				selectedNodes[nodes[i]->nodeNumber] = nodes[i];
//			}
//			NodeAction *newAction = new NodeAction(selectedNodes);
//			newAction->SetSelectionLayer(this);
//			undoStack.push(newAction);
//			emit undoAvailable(true);
//		} else {

//			// Don't want to double-select nodes
//			std::map<unsigned int, Node*> actualSelection;
//			for (unsigned int i=0; i<nodes.size(); i++)
//			{
//				if (selectedNodes.count(nodes[i]->nodeNumber) == 0)
//				{
//					selectedNodes[nodes[i]->nodeNumber] = nodes[i];
//					actualSelection[nodes[i]->nodeNumber] = nodes[i];
//				}
//			}

//			if (actualSelection.size() > 0)
//			{
//				NodeAction *newAction = new NodeAction(actualSelection);
//				newAction->SetSelectionLayer(this);
//				undoStack.push(newAction);
//				emit undoAvailable(true);
//			}

//		}

//		// Clear the Redo stack
//		for (unsigned int i=0; i<redoStack.size(); i++)
//		{
//			Action *curr = redoStack.top();
//			redoStack.pop();
//			if (curr)
//				delete curr;
//		}
//		redoAvailable(false);

//		emit numNodesSelected(selectedNodes.size());
//		UpdateVertexBuffer();
//	}
//}


//void SelectionLayer::SelectElements(std::vector<Element *> elements)
//{
//	time_t startTime;
//	startTime = clock();
//	if (elements.size() > 0)
//	{
//		// First, check for duplicate elements
//		std::vector<Element*> actualSelection;
//		if (selectedElements.size() == 0)
//		{
//			actualSelection = elements;
//		} else {
//			for (unsigned int i=0; i<elements.size(); i++)
//			{
//				if (selectedElements.count(elements[i]->elementNumber) == 0)
//				{
//					actualSelection.push_back(elements[i]);
//				}
//			}
//		}

//		if (actualSelection.size() > 0)
//		{
//			// Now that we've got the unique list of elements, let's add them to the selectedElements list
//			std::map<unsigned int, Node*> additionalNodes;
//			std::map<unsigned int, Element*> additionalElements;
//			for (unsigned int i=0; i<actualSelection.size(); i++)
//			{
//				// Add all of the elements, checking for additionally needed nodes along the way
//				selectedElements[actualSelection[i]->elementNumber] = actualSelection[i];
//				additionalElements[actualSelection[i]->elementNumber] = actualSelection[i];

//				if (selectedNodes.count(actualSelection[i]->n1->nodeNumber) == 0)
//					additionalNodes[actualSelection[i]->n1->nodeNumber] = actualSelection[i]->n1;
//				if (selectedNodes.count(actualSelection[i]->n2->nodeNumber) == 0)
//					additionalNodes[actualSelection[i]->n2->nodeNumber] = actualSelection[i]->n2;
//				if (selectedNodes.count(actualSelection[i]->n3->nodeNumber) == 0)
//					additionalNodes[actualSelection[i]->n3->nodeNumber] = actualSelection[i]->n3;
//			}

//			// Now add the new Elements
//			if (additionalElements.size() > 0)
//			{
//				ElementAction *newAction = new ElementAction(additionalNodes, additionalElements);
//				newAction->SetSelectionLayer(this);
//				newAction->RedoAction();
//				undoStack.push(newAction);
//				emit undoAvailable(true);
//				UpdateVertexBuffer();
//				UpdateIndexBuffer();
//			}
//		}
//	}
//	DEBUG("SelectionLayer time: " << clock()-startTime);
//}


///**
// * @brief Undo the previous action
// *
// * Undoes the previous Action by popping the undo stack, performing the popped Action's
// * undo function, and pushing that Action onto the redo stack.
// *
// */
//void SelectionLayer::undo()
//{
//	// Undo the last selection/deselection
//	if (undoStack.size() > 0)
//	{
//		Action *undoAction = undoStack.top();
//		undoStack.pop();
//		if (undoAction)
//		{
//			undoAction->UndoAction();
//			redoStack.push(undoAction);
//			emit redoAvailable(true);
//		}
//	}

//	if (undoStack.size() == 0)
//		emit undoAvailable(false);

//}


///**
// * @brief Redo the previous undo
// *
// * Redoes the last undone Action by popping the redo stack, performing the popped Action's
// * redo function, and pushing that Action onto the undo stack.
// *
// */
//void SelectionLayer::redo()
//{
//	if (redoStack.size() > 0)
//	{
//		Action *redoAction = redoStack.top();
//		redoStack.pop();
//		if (redoAction)
//		{
//			redoAction->RedoAction();
//			undoStack.push(redoAction);
//			emit undoAvailable(true);
//		}
//	}

//	if (redoStack.size() == 0)
//		emit redoAvailable(false);
//}
