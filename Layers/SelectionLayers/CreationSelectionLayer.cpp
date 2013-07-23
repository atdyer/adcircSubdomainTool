#include "CreationSelectionLayer.h"

CreationSelectionLayer::CreationSelectionLayer()
{
	activeTool = 0;
	circleTool = 0;

	glLoaded = false;
	camera = 0;
	outlineShader = 0;
	fillShader = 0;
	boundaryShader = 0;
}


CreationSelectionLayer::~CreationSelectionLayer()
{
	if (outlineShader)
		delete outlineShader;
	if (fillShader)
		delete fillShader;
	if (boundaryShader)
		delete boundaryShader;
}


void CreationSelectionLayer::Draw()
{

}


void CreationSelectionLayer::LoadDataToGPU()
{
	/* Make sure we've got all of the necessary Buffer Objects created */
	if (!glLoaded)
		InitializeGL();

	/* Make sure initialization succeeded */
	if (glLoaded)
	{
		/* Load the connectivity data (elements) to the GPU, getting rid of any data that's already there */
		const size_t IndexBufferSize = 3*sizeof(GLuint)*selectedElements.size();
		if (IndexBufferSize && VAOId && IBOId)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
			GLuint* glElementData = (GLuint*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
			if (glElementData)
			{
				Element* currElement;
				int i=0;
				for (std::vector<Element*>::iterator it=selectedElements.begin(); it != selectedElements.end(); ++it, i++)
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
	if (outlineShader)
		outlineShader->SetCamera(newCamera);
	if (fillShader)
		fillShader->SetCamera(newCamera);
	if (boundaryShader)
		boundaryShader->SetCamera(newCamera);
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
	return selectedElements.size();
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

	VBOId = terrainLayer->GetVBOId();

	if (circleTool)
		circleTool->SetTerrainLayer(newLayer);
}


void CreationSelectionLayer::UseTool(int toolID)
{

}


void CreationSelectionLayer::MouseClick(int x, int y)
{

}


void CreationSelectionLayer::MouseMove(int dx, int dy)
{

}


void CreationSelectionLayer::MouseRelease(int x, int y)
{

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
