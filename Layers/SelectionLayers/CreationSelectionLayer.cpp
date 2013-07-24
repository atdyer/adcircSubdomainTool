#include "CreationSelectionLayer.h"

CreationSelectionLayer::CreationSelectionLayer()
{
	activeTool = 1;
	circleTool = 0;

	selectedElements = 0;

	glLoaded = false;
	camera = 0;
	VAOId = 0;
	VBOId = 0;
	IBOId = 0;
	outlineShader = 0;
	fillShader = 0;
	boundaryShader = 0;

	mousePressed = false;
	CreateCircleTool();
}


/**
 * @brief Deconstructor that cleans up data on the GPU and any allocated data
 *
 * Cleans up any data that has been created by this class. Marks data for
 * deletion on the GPU. Note that we are not responsible for cleaning up
 * the VBO.
 *
 */
CreationSelectionLayer::~CreationSelectionLayer()
{
	DEBUG("Deleting Creation Selection Layer. Layer ID: " << GetID());

	/* Clean up shaders */
	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
	if (boundaryShader)
		delete boundaryShader;

	// Clean up the OpenGL stuff
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	/* Note that we aren't responsible for cleaning up the VBO */

	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);

	/* Clean up the undo/redo stacks */
	while(!undoStack.empty())
	{
		std::vector<Element*>* curr = undoStack.top();
		undoStack.pop();
		if (curr)
			delete curr;
	}

	while(!redoStack.empty())
	{
		std::vector<Element*>* curr = redoStack.top();
		redoStack.pop();
		if (curr)
			delete curr;
	}
}


/**
 * @brief Draws the selected Elements
 *
 * Draws the currently selected Elements (fill and then outline), as well as boundary
 * segments if they are defined.
 *
 */
void CreationSelectionLayer::Draw()
{
	if (glLoaded && selectedElements)
	{
		glBindVertexArray(VAOId);

		DEBUG("Selection: " << VBOId << " " << IBOId << " " << VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				glDrawElements(GL_TRIANGLES, selectedElements->size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				glDrawElements(GL_TRIANGLES, selectedElements->size()*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		// Draw boundaries here
	}
}


/**
 * @brief Loads the currently selected Element data to the GPU
 *
 * Loads the currently selected Element data to the GPU, getting rid of any element
 * data that is already there.
 *
 */
void CreationSelectionLayer::LoadDataToGPU()
{
	/* Make sure we've got all of the necessary Buffer Objects created */
	if (!glLoaded)
		InitializeGL();

	/* Make sure initialization succeeded */
	if (glLoaded && selectedElements)
	{
		/* Load the connectivity data (elements) to the GPU, getting rid of any data that's already there */
		const size_t IndexBufferSize = 3*sizeof(GLuint)*selectedElements->size();
		if (IndexBufferSize && VAOId && IBOId)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
			GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glElementData)
			{
				Element* currElement;
				int i=0;
				for (std::vector<Element*>::iterator it=selectedElements->begin(); it != selectedElements->end(); ++it, i++)
				{
					currElement = *it;
					glElementData[3*i+0] = (GLuint)currElement->n1->nodeNumber-1;
					glElementData[3*i+1] = (GLuint)currElement->n2->nodeNumber-1;
					glElementData[3*i+2] = (GLuint)currElement->n3->nodeNumber-1;
				}
			} else {
				glLoaded = false;
				DEBUG("ERROR: Mapping index buffer for Subdomain Creation Selection Layer " << GetID());
				emit emitMessage("<p style:color='red'><strong>Error: Unable to load index data to GPU (Subdomain Creation Selection Layer)</strong>");
				return;
			}

			if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
			{
				glLoaded = false;
				DEBUG("ERROR: Unmapping index buffer for Subdomain Creation Selection Layer " << GetID());
				return;
			}
		}

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
			} else {
				DEBUG("Subdomain Creation Selection Layer Data Not Loaded");
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("CreationSelectionLayer OpenGL Error: " << errString);
			glLoaded = false;
		}

		emit Refreshed();
	}
}


/**
 * @brief Sets the camera used during drawing operations
 *
 * Sets the camera being used during drawing operations
 *
 * @param cam Pointer to the GLCamera object
 */
void CreationSelectionLayer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;

	/* Set the camera for the shaders */
	if (outlineShader)
		outlineShader->SetCamera(newCamera);
	if (fillShader)
		fillShader->SetCamera(newCamera);
	if (boundaryShader)
		boundaryShader->SetCamera(newCamera);

	/* Set the camera for the tools */
	if (circleTool)
		circleTool->SetCamera(newCamera);
}


/**
 * @brief Returns the number of Elements that are currently selected
 *
 * Returns the number of Elements that are currently selected
 *
 * @return The number of Elements that are currently selected
 */
unsigned int CreationSelectionLayer::GetNumElementsSelected()
{
	if (selectedElements)
		return selectedElements->size();
	return 0;
}


/**
 * @brief Sets the Terrain Layer that all actions and selections will be performed on
 *
 * Sets the Terrain Layer that all actions and selections will be performed on. This is
 * where the selection layer gets the vertex data (VBOId) for drawing.
 *
 * @param newLayer Pointer to the TerrainLayer object
 */
void CreationSelectionLayer::SetTerrainLayer(TerrainLayer *newLayer)
{
	terrainLayer = newLayer;

	connect(terrainLayer, SIGNAL(finishedLoadingToGPU()), this, SLOT(TerrainDataLoaded()));

	if (circleTool)
		circleTool->SetTerrainLayer(newLayer);
}


/**
 * @brief Select the Selection Tool to be used for the next interaction
 *
 * Select the Selection Tool to be used for the next interaction:
 * - 0 - Stop using tools
 * - 1 - CircleTool - Select Elements inside of a circle
 *
 * @param toolID The tool to be used for the next interaction
 */
void CreationSelectionLayer::UseTool(int toolID)
{
	/* Make sure we're trying to select an ID that we've got */
	if (toolID >= 0 && toolID <= AVAILABLETOOLS)
	{
		activeTool = toolID;

		/* If the tool hasn't been created yet, create it now */
		if (activeTool == CIRCLETOOLINDEX && !circleTool)
			circleTool = new CircleTool();
	}
}


/**
 * @brief Passes the mouse click coordinates to the currently active selection tool
 *
 * Passes the mouse click coordinates to the currently active selection tool.
 *
 * @param x x-coordinate (pixels)
 * @param y y-coordinate (pixels)
 */
void CreationSelectionLayer::MouseClick(int x, int y)
{
	mousePressed = true;

	if (activeTool == CIRCLETOOLINDEX && circleTool)
		circleTool->SetCenter(x, y);
}


/**
 * @brief Passes the mouse coordinates to the currently active selection tool when the mouse is moved
 *
 * Passes the mouse coordinates to the currently active selection tool when the mouse is moved
 *
 * @param x x-coordinate (pixels)
 * @param y y-coordinate (pixels)
 */
void CreationSelectionLayer::MouseMove(int x, int y)
{
	if (mousePressed && activeTool == CIRCLETOOLINDEX && circleTool)
		circleTool->SetRadiusPoint(x, y);
}


/**
 * @brief Passes the mouse coordinates to the currently active selection tool when the mouse click is released
 *
 * Passes the mouse coordinates to the currently active selection tool when the mouse click is released
 *
 * @param x x-coordinate (pixels)
 * @param y y-coordinate (pixels)
 */
void CreationSelectionLayer::MouseRelease(int x, int y)
{
	mousePressed = false;

	if (activeTool == CIRCLETOOLINDEX && circleTool)
		circleTool->CircleFinished();
}


void CreationSelectionLayer::WindowSizeChanged(float w, float h)
{
	if (circleTool)
		circleTool->SetViewportSize(w, h);
}


void CreationSelectionLayer::Undo()
{

}


void CreationSelectionLayer::Redo()
{

}


/**
 * @brief Initializes the Buffer Objects and Shaders objects necessary for drawing the
 * selection layer
 *
 * This function initializes the Buffer Objects in the OpenGL context as well as the
 * Shader objects necessary for drawing the selection layer. Default transparent grays
 * are used for color.
 *
 * This layer makes use of the vertex data that is already on the GPU from the TerrainLayer.
 * Therefore, we only need to create a new Vertex Array Object and Index Buffer Object.
 *
 */
void CreationSelectionLayer::InitializeGL()
{
	/* Only perform initialization if we have a VBO from a TerrainLayer */
	if (VBOId)
	{
		/* Create new shaders */
		outlineShader = new SolidShader();
		fillShader = new SolidShader();
		boundaryShader = new SolidShader();

		/* Set the shader properties */
		fillShader->SetColor(0.4, 0.4, 0.4, 0.4);
		outlineShader->SetColor(0.2, 0.2, 0.2, 0.2);
		boundaryShader->SetColor(0.0, 0.0, 0.0, 0.8);
		if (camera)
		{
			fillShader->SetCamera(camera);
			outlineShader->SetCamera(camera);
			boundaryShader->SetCamera(camera);
		}

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &IBOId);

		/* Bind the VBO and IBO to the VAO */
		glBindVertexArray(VAOId);
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				DEBUG("Subdomain Creation Selection Layer Initialized");
				glLoaded = true;
			} else {
				DEBUG("Subdomain Creation Selection Layer Not Initialized");
				glLoaded = false;
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("Subdomain Creation Selection Layer OpenGL Error: " << errString);
			glLoaded = false;
		}
	} else {
		DEBUG("Subdomain Creation Selection Layer GL not initialized: TerrainLayer not set");
		glLoaded = false;
	}
}


/**
 * @brief Creates the circle selection tool
 *
 * Creates the circle selection tool. We hook up the signal/slot mechanism here
 * so that this layer knows when the tool has finished finding all elements and
 * is ready to be queried.
 *
 */
void CreationSelectionLayer::CreateCircleTool()
{
	if (!circleTool)
		circleTool = new CircleTool();

	circleTool->SetTerrainLayer(terrainLayer);
	circleTool->SetCamera(camera);
	connect(circleTool, SIGNAL(FinishedSearching()), this, SLOT(CircleToolFinishedSearching()));
}


void CreationSelectionLayer::TerrainDataLoaded()
{
	VBOId = terrainLayer->GetVBOId();
}


/**
 * @brief Queries the circle tool for currently selected elements
 *
 * Queries the circle tool for currently selected elements. Creates a new list
 * of currently selected elements by combining the currently selected elements
 * with the newly selected ones.
 *
 * The previously selected elements list is pushed onto the undo stack and
 * the new list is made the currently selected elements list.
 *
 */
void CreationSelectionLayer::CircleToolFinishedSearching()
{
	if (!selectedElements)
		selectedElements = new std::vector<Element*>();
	if (circleTool)
	{
		std::vector<Element*> *newList = new std::vector<Element*>(circleTool->GetSelectedElements());
		DEBUG("Found " << newList->size() << " elements");
		if (newList->size() > 0)
		{
			if (selectedElements->size() > 0)
			{
				/* There are currently selected elements, so combine the lists */
				newList->reserve(newList->size() + selectedElements->size());
				newList->insert(newList->end(), selectedElements->begin(), selectedElements->end());

				/* Push the old list onto the undo stack */
				undoStack.push(selectedElements);

				/* Get rid of any duplicates in the newly created list */
				std::vector<Element*>::iterator it;
				it = std::unique(newList->begin(), newList->end());
				newList->resize(std::distance(newList->begin(), it));

				/* Make the currently selected list the newly created list */
				selectedElements = newList;
			} else {
				/* There aren't any currently selected elements, so just go ahead and
				 * make the newly created list the currently selected list. Push back
				 * the empty list so that the empty state is saved in the undo stack */
				undoStack.push(selectedElements);
				selectedElements = newList;
			}

			/* Update the data being displayed */
			LoadDataToGPU();
		} else {
			/* No elements were selected, so just go ahead and delete the new list */
			delete newList;
		}
	}
}