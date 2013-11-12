#include "Fort14_new.h"

/**
 * @brief Default Constructor
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(QObject *parent) :
	QObject(parent),
	domainName(),
	elements(),
	elevationBoundaries(),
	flowBoundaries(),
	max(0.0),
	maxX(0.0),
	maxY(0.0),
	maxZ(0.0),
	midX(0.0),
	midY(0.0),
	minX(0.0),
	minY(0.0),
	minZ(0.0),
	nodes(),
	numElements(0),
	numNodes(0),
	progressBar(0),
	projectFile(0),
	quadtree(0),
	readingLock(false),
	glLoaded(false),
	camera(0),
	outlineShader(0), fillShader(0), boundaryShader(0),
	solidOutline(0), solidFill(0), solidBoundary(0),
	gradientOutline(0), gradientFill(0), gradientBoundary(0),
	VAOId(0), VBOId(0), IBOId(0)
{

}


/**
 * @brief Constructor to be used if this fort.14 file is for a full domain
 * @param projectFile The project file to get file data from
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(),
	elements(),
	elevationBoundaries(),
	flowBoundaries(),
	max(0.0),
	maxX(0.0),
	maxY(0.0),
	maxZ(0.0),
	midX(0.0),
	midY(0.0),
	minX(0.0),
	minY(0.0),
	minZ(0.0),
	nodes(),
	numElements(0),
	numNodes(0),
	progressBar(0),
	projectFile(projectFile),
	quadtree(0),
	readingLock(false),
	glLoaded(false),
	camera(0),
	solidOutline(0), solidFill(0), solidBoundary(0),
	gradientOutline(0), gradientFill(0), gradientBoundary(0),
	VAOId(0), VBOId(0), IBOId(0)
{
	ReadFile();
}

/**
 * @brief Constructor to be used if this fort.14 file is for a subdomain
 * @param domainName The subdomain's name
 * @param projectFile The project file to get file data from
 * @param parent The parent Domain object
 */
Fort14_new::Fort14_new(QString domainName, ProjectFile_new *projectFile, QObject *parent) :
	QObject(parent),
	domainName(domainName),
	elements(),
	elevationBoundaries(),
	flowBoundaries(),
	max(0.0),
	maxX(0.0),
	maxY(0.0),
	maxZ(0.0),
	midX(0.0),
	midY(0.0),
	minX(0.0),
	minY(0.0),
	minZ(0.0),
	nodes(),
	numElements(0),
	numNodes(0),
	progressBar(0),
	projectFile(projectFile),
	quadtree(0),
	readingLock(false),
	glLoaded(false),
	camera(0),
	solidOutline(0), solidFill(0), solidBoundary(0),
	gradientOutline(0), gradientFill(0), gradientBoundary(0),
	VAOId(0), VBOId(0), IBOId(0)
{
	ReadFile();
}


Fort14_new::~Fort14_new()
{
	if (quadtree)
		delete quadtree;

	if (solidOutline)
		delete solidOutline;
	if (solidFill)
		delete solidFill;
	if (solidBoundary)
		delete solidBoundary;
	if (gradientOutline)
		delete gradientOutline;
	if (gradientFill)
		delete gradientFill;
	if (gradientBoundary)
		delete gradientBoundary;

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	if (VBOId)
		glDeleteBuffers(1, &VBOId);
	if (VAOId)
		glDeleteBuffers(1, &VAOId);
	if (IBOId)
		glDeleteBuffers(1, &IBOId);
}


void Fort14_new::Draw()
{
	if (glLoaded)
	{
		glBindVertexArray(VAOId);

		if (fillShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			if (fillShader->Use())
				glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		if (outlineShader)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			if (outlineShader->Use())
				glDrawElements(GL_TRIANGLES, numElements*3, GL_UNSIGNED_INT, (GLvoid*)0);
		}

		/* Need to get this to work with the multiple boundary types */
//		if (boundaryShader)
//		{
//			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//			glLineWidth(3.0);
//			if (boundaryShader->Use())
//				glDrawElements(GL_LINE_STRIP, boundaryNodes.size(), GL_UNSIGNED_INT, (GLvoid*)(0 + sizeof(GLuint)*numElements*3));
//			glLineWidth(1.0);
//		}

		glBindVertexArray(0);
		glUseProgram(0);
	}
}


Element* Fort14_new::FindElement(float xGL, float yGL)
{
	if (quadtree)
		return quadtree->FindElement(xGL, yGL);
	return 0;
}


std::vector<Element*> Fort14_new::FindElementsInCircle(float x, float y, float radius)
{
	if (quadtree)
	{
		return quadtree->FindElementsInCircle(x, y, radius);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


std::vector<Element*> Fort14_new::FindElementsInRectangle(float l, float r, float b, float t)
{
	if (quadtree)
	{
		return quadtree->FindElementsInRectangle(l, r, b, t);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


std::vector<Element*> Fort14_new::FindElementsInPolygon(std::vector<Point> polyLine)
{
	if (quadtree)
	{
		return quadtree->FindElementsInPolygon(polyLine);
	} else {
		std::vector<Element*> fail;
		return fail;
	}
}


ShaderType Fort14_new::GetBoundaryShaderType()
{
	if (boundaryShader)
		return boundaryShader->GetShaderType();
	return NoShaderType;
}


std::vector<Element>* Fort14_new::GetElements()
{
	return &elements;
}


QString Fort14_new::GetFilePath()
{
	if (projectFile)
	{
		if (domainName.isEmpty())
		{
			return projectFile->GetFullDomainFort14();
		}
		else
		{
			return projectFile->GetSubDomainFort14(domainName);
		}
	}
	return QString();
}


ShaderType Fort14_new::GetFillShaderType()
{
	if (fillShader)
		return fillShader->GetShaderType();
	return NoShaderType;
}


QGradientStops Fort14_new::GetGradientBoundaryColors()
{
	if (gradientBoundary)
		return gradientBoundary->GetGradientStops();
	return QGradientStops();
}


QGradientStops Fort14_new::GetGradientFillColors()
{
	if (gradientFill)
		return gradientFill->GetGradientStops();
	return QGradientStops();
}


QGradientStops Fort14_new::GetGradientOutlineColors()
{
	if (gradientOutline)
		return gradientOutline->GetGradientStops();
	return QGradientStops();
}


float Fort14_new::GetMaxX()
{
	return maxX;
}


float Fort14_new::GetMaxY()
{
	return maxY;
}


float Fort14_new::GetMaxZ()
{
	return maxZ;
}


float Fort14_new::GetMinX()
{
	return minX;
}


float Fort14_new::GetMinY()
{
	return minY;
}


float Fort14_new::GetMinZ()
{
	return minZ;
}


ShaderType Fort14_new::GetOutlineShaderType()
{
	if (outlineShader)
		return outlineShader->GetShaderType();
	return NoShaderType;
}


float Fort14_new::GetUnprojectedX(float x)
{
	return x*max + midX;
}


float Fort14_new::GetUnprojectedY(float y)
{
	return y*max + midY;
}


std::vector<Element*> Fort14_new::GetSelectedElements()
{

}


QColor Fort14_new::GetSolidBoundaryColor()
{
	if (solidBoundary)
		return solidBoundary->GetShaderProperties();
	return QColor();
}


QColor Fort14_new::GetSolidFillColor()
{
	if (solidFill)
		return solidFill->GetShaderProperties();
	return QColor();
}


QColor Fort14_new::GetSolidOutlineColor()
{
	if (solidOutline)
		return solidOutline->GetShaderProperties();
	return QColor();
}


void Fort14_new::SetCamera(GLCamera *camera)
{
	this->camera = camera;
}


void Fort14_new::SetGradientBoundaryColors(QGradientStops newStops)
{
	if (!gradientBoundary)
	{
		gradientBoundary = new GradientShader();
		gradientBoundary->SetCamera(camera);
		gradientBoundary->SetGradientRange(minZ, maxZ);
	}

	gradientBoundary->SetGradientStops(newStops);

	boundaryShader = gradientBoundary;
}


void Fort14_new::SetGradientFillColors(QGradientStops newStops)
{
	if (!gradientFill)
	{
		gradientFill = new GradientShader();
		gradientFill->SetCamera(camera);
		gradientFill->SetGradientRange(minZ, maxZ);
	}

	gradientFill->SetGradientStops(newStops);

	fillShader = gradientFill;
}


void Fort14_new::SetGradientOutlineColors(QGradientStops newStops)
{
	if (!gradientOutline)
	{
		gradientOutline = new GradientShader();
		gradientOutline->SetCamera(camera);
		gradientOutline->SetGradientRange(minZ, maxZ);
	}

	gradientOutline->SetGradientStops(newStops);

	outlineShader = gradientOutline;
}


void Fort14_new::SetProgressBar(QProgressBar *newBar)
{
	progressBar = newBar;
}


void Fort14_new::SetSolidBoundaryColor(QColor newColor)
{
	if (!solidBoundary)
	{
		solidBoundary = new SolidShader();
		solidBoundary->SetCamera(camera);
	}

	solidBoundary->SetColor(newColor);
	boundaryShader = solidBoundary;
}


void Fort14_new::SetSolidFillColor(QColor newColor)
{
	if (!solidFill)
	{
		solidFill = new SolidShader();
		solidFill->SetCamera(camera);
	}

	solidFill->SetColor(newColor);
	fillShader = solidFill;
}


void Fort14_new::SetSolidOutlineColor(QColor newColor)
{
	if (!solidOutline)
	{
		solidOutline = new SolidShader();
		solidOutline->SetCamera(camera);
	}

	solidOutline->SetColor(newColor);
	outlineShader = solidOutline;
}


void Fort14_new::CreateDefaultShaders()
{
	SetSolidFillColor(QColor(0.1*255, 0.8*255, 0.1*255, 1.0*255));
	SetSolidOutlineColor(QColor(0.2*255, 0.2*255, 0.2*255, 0.1*255));
}


void Fort14_new::LoadGL()
{
	CreateDefaultShaders();

	if (!glLoaded)
	{
		const size_t VertexBufferSize = 4*sizeof(GLfloat)*numNodes;

		glGenVertexArrays(1, &VAOId);
		glGenBuffers(1, &VBOId);
		glGenBuffers(1, &IBOId);

		glBindVertexArray(VAOId);

		// Send Vertex Data
		glBindBuffer(GL_ARRAY_BUFFER, VBOId);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), 0);
		glBufferData(GL_ARRAY_BUFFER, VertexBufferSize, NULL, GL_STATIC_DRAW);
		GLfloat* glNodeData = (GLfloat *)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glNodeData)
		{
			for (unsigned int i=0; i<numNodes; i++)
			{
				glNodeData[4*i+0] = (GLfloat)nodes[i].normX;
				glNodeData[4*i+1] = (GLfloat)nodes[i].normY;
				glNodeData[4*i+2] = (GLfloat)nodes[i].z;
				glNodeData[4*i+3] = (GLfloat)1.0;
			}
		} else {
			glLoaded = false;
			return;
		}

		if (glUnmapBuffer(GL_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			DEBUG("ERROR: Unmapping vertex buffer for fort.14");
			return;
		}

		// Send Index Data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBOId);
		const size_t IndexBufferSize = 3*sizeof(GLuint)*numElements; // + sizeof(GLuint)*boundaryNodes.size();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, IndexBufferSize, NULL, GL_STATIC_DRAW);
		GLuint* glElementData = (GLuint *)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		if (glElementData)
		{
			for (unsigned int i=0; i<numElements; i++)
			{
				glElementData[3*i+0] = (GLuint)elements[i].n1->nodeNumber-1;
				glElementData[3*i+1] = (GLuint)elements[i].n2->nodeNumber-1;
				glElementData[3*i+2] = (GLuint)elements[i].n3->nodeNumber-1;
			}
//			for (unsigned int i=0; i<boundaryNodes.size(); i++)
//			{
//				glElementData[3*numElements+i] = boundaryNodes[i]-1;
//			}
		} else {
			glLoaded = false;
			return;
		}

		if (glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER) == GL_FALSE)
		{
			glLoaded = false;
			DEBUG("ERROR: Unmapping index buffer for fort.14");
			return;
		}

		glBindVertexArray(0);

		GLenum errorCheck = glGetError();
		if (errorCheck == GL_NO_ERROR)
		{
			if (VAOId && VBOId && IBOId)
			{
				glLoaded = true;
				emit DataLoadedToGPU(VBOId);
			}
		} else {
			const GLubyte *errString = gluErrorString(errorCheck);
			DEBUG("OpenGL Error: " << errString);
			glLoaded = false;
		}
	}
}


void Fort14_new::PopulateQuadtree()
{
	if (!quadtree && nodes.size() && elements.size())
	{
		std::cout << minX << ", " << maxX << std::endl;
		quadtree = new Quadtree(nodes, elements, 50, (minX-midX)/max, (maxX-midX)/max, (minY-midY)/max, (maxY-midY)/max);
	}
}


void Fort14_new::ReadFile()
{
	if (!readingLock)
	{
		std::cout << "Launching from thread: " << this->thread() << std::endl;
		QString filePath = GetFilePath();
		if (QFile(filePath).exists())
		{
			QThread *thread = new QThread();
			Fort14Reader *worker = new Fort14Reader(filePath, &nodes, &elements, &elevationBoundaries,
								&flowBoundaries, true, this);

			// Move to thread and prepare to start reading
			worker->moveToThread(thread);
			connect(thread, SIGNAL(started()), worker, SLOT(ReadFile()));


			// Hook up signals that communicate data between threads
			if (progressBar)
			{
				connect(worker, SIGNAL(StartedReading()), progressBar, SLOT(show()));
				connect(worker, SIGNAL(FinishedReading()), progressBar, SLOT(hide()));
			}
			connect(worker, SIGNAL(Progress(int)), this, SLOT(Progress(int)));
			connect(worker, SIGNAL(FoundDomainBounds(float,float,float,float,float,float)),
				this, SLOT(SetDomainBounds(float,float,float,float,float,float)));


			// Hook up finishing signals
			connect(worker, SIGNAL(FinishedReading()), this, SLOT(FinishedReading()), Qt::UniqueConnection);
			connect(worker, SIGNAL(FinishedReading()), thread, SLOT(quit()));
			connect(worker, SIGNAL(FinishedReading()), worker, SLOT(deleteLater()));
			connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

			LockFile();
			thread->start();
		}
	} else {
		std::cout << "Unable to read with reading lock on" << std::endl;
	}
}


void Fort14_new::UnlockFile()
{
	std::cout << "\n-----\nUnlocking file.\nNode Count: " << nodes.size() << "\nElement Count: " << elements.size() <<
		     "\n-----" << std::endl;
	readingLock = false;
}


void Fort14_new::SelectCircle(int x, int y, int radius)
{

}


void Fort14_new::SelectPoint(int x, int y)
{

}


void Fort14_new::SelectPolygon(std::vector<Point> polyLine)
{

}


void Fort14_new::SelectRectangle(int l, int r, int b, int t)
{

}


void Fort14_new::LockFile()
{
	std::cout << "\n-----\nLocking file.\nNode Count: " << nodes.size() << "\nElement Count: " << elements.size() <<
		     "\n-----" << std::endl;
	readingLock = true;
}


void Fort14_new::FinishedReading()
{
	numNodes = nodes.size();
	numElements = elements.size();

	UnlockFile();
	PopulateQuadtree();
	LoadGL();
}


void Fort14_new::Progress(int percent)
{
	if (progressBar)
	{
		progressBar->setValue(percent);
		if (percent == 100)
		{
			progressBar->reset();
		}
	}
}


void Fort14_new::SetDomainBounds(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
	this->minX = minX;
	this->minY = minY;
	this->minZ = minZ;
	this->maxX = maxX;
	this->maxY = maxY;
	this->maxZ = maxZ;
	this->midX = minX + (maxX - minX) / 2.0;
	this->midY = minY + (maxY - minY) / 2.0;
	this->max = fmax(maxX-minX, maxY-minY);
}
