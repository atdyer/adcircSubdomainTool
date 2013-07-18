#include "Layer.h"

// Initialize static members
unsigned int	Layer::layerCount = 0;
unsigned int	Layer::nextID = 1;


/**
 * @brief Constructor for the Layer class
 *
 * Increments the layerCount and assigns the new layer a unique layerID
 *
 */
Layer::Layer(QObject *parent) : QObject(parent)
{
	layerCount++;
	layerID = nextID;
	nextID++;

	camera = 0;
	solidOutline = 0;
	solidFill = 0;
	gradientOutline = 0;
	gradientFill = 0;
}


/**
 * @brief Destructor for the Layer class
 *
 * Decrements the layerCount and emits the finishedThreading signal
 *
 */
Layer::~Layer()
{
	layerCount--;

	if (solidOutline)
		delete solidOutline;
	if (solidFill)
		delete solidFill;
	if (gradientOutline)
		delete gradientOutline;
	if (gradientFill)
		delete gradientFill;
}


/**
 * @brief Returns the unique ID associated with the Layer object
 * @return The unique ID associated with the Layer object
 */
unsigned int Layer::GetID()
{
	return layerID;
}


/**
 * @brief Returns the total number of layers that currently exist
 * @return The number of Layer objects that currently exist
 */
unsigned int Layer::GetNumLayers()
{
	return layerCount;
}


SolidShaderProperties Layer::GetSolidOutline()
{
	if (solidOutline)
		return solidOutline->GetShaderProperties();
	return SolidShaderProperties();
}


SolidShaderProperties Layer::GetSolidFill()
{
	if (solidFill)
		return solidFill->GetShaderProperties();
	return SolidShaderProperties();
}


GradientShaderProperties Layer::GetGradientOutline()
{
	if (gradientOutline)
		return gradientOutline->GetShaderProperties();
	return GradientShaderProperties();
}


GradientShaderProperties Layer::GetGradientFill()
{
	if (gradientFill)
		return gradientFill->GetShaderProperties();
	return GradientShaderProperties();
}


/**
 * @brief Sets the GLCamera object to be used when drawing this Layer
 *
 * Sets the GLCamera object to be used when drawing this Layer. The Layer does
 * not take ownership of the GLCamera object.
 *
 * @param newCamera Pointer to the GLCamera object
 */
void Layer::SetCamera(GLCamera *newCamera)
{
	camera = newCamera;
	if (solidOutline)
		solidOutline->SetCamera(camera);
	if (solidFill)
		solidFill->SetCamera(camera);
	if (gradientOutline)
		gradientOutline->SetCamera(camera);
	if (gradientFill)
		gradientFill->SetCamera(camera);
}


/**
 * @brief Sets the solid color used for drawing this Layer's outline
 *
 * Sets the solid color used for drawing this Layer's outline. The shader
 * is owned by the Layer.
 *
 * @param r Red value [0.0 - 1.0]
 * @param g Green value [0.0 - 1.0]
 * @param b Blue value [0.0 - 1.0]
 * @param a Alpha value [0.0 - 1.0]
 */
void Layer::UseSolidOutlineShader(float r, float g, float b, float a)
{
	if (!solidOutline)
		solidOutline = new SolidShader();

	solidOutline->SetCamera(camera);
	solidOutline->SetColor(r, g, b, a);
	outlineShader = solidOutline;
}


/**
 * @brief Sets the solid color used for drawing this Layer's fill
 *
 * Sets the solid color used for drawing this Layer's fill. The shader
 * is owned by the Layer
 *
 * @param r Red value [0.0 - 1.0]
 * @param g Green value [0.0 - 1.0]
 * @param b Blue value [0.0 - 1.0]
 * @param a Alpha value [0.0 - 1.0]
 */
void Layer::UseSolidFillShader(float r, float g, float b, float a)
{
	if (!solidFill)
		solidFill = new SolidShader();

	solidFill->SetCamera(camera);
	solidFill->SetColor(r, g, b, a);
	fillShader = solidFill;
}


/**
 * @brief Sets the gradient used for drawing this Layer's outline
 *
 * Sets the gradient used for drawing the Layer's outline. The shader
 * is owned by the Layer
 *
 * @param lowColor [r, g, b, a] values
 * @param highColor [r, g, b, a] values
 * @param lowVal The low gradient value
 * @param highVal The high gradient value
 */
void Layer::UseGradientOutlineShader(float lowColor[], float highColor[], float lowVal, float highVal)
{
	if (!gradientOutline)
		gradientOutline = new GradientShader();

	gradientOutline->SetCamera(camera);
	gradientOutline->SetLowColor(lowColor[0], lowColor[1], lowColor[2], lowColor[3]);
	gradientOutline->SetHighColor(highColor[0], highColor[1], highColor[2], highColor[3]);
	gradientOutline->SetLowValue(lowVal);
	gradientOutline->SetHighValue(highVal);
	outlineShader = gradientOutline;
}


/**
 * @brief Sets the gradient used for drawing this Layer's fill
 *
 * Sets the gradient used for drawing this Layer's fill. The shader
 * is owned by the Layer
 *
 * @param lowColor [r, g, b, a] values
 * @param highColor [r, g, b, a] values
 * @param lowVal The low gradient value
 * @param highVal The high gradient value
 */
void Layer::UseGradientFillShader(float lowColor[], float highColor[], float lowVal, float highVal)
{
	if (!gradientFill)
		gradientFill = new GradientShader();

	gradientFill->SetCamera(camera);
	gradientFill->SetLowColor(lowColor[0], lowColor[1], lowColor[2], lowColor[3]);
	gradientFill->SetHighColor(highColor[0], highColor[1], highColor[2], highColor[3]);
	gradientFill->SetLowValue(lowVal);
	gradientFill->SetHighValue(highVal);
	fillShader = gradientFill;
}
