#ifndef GRADIENTSHADER_H
#define GRADIENTSHADER_H

#include <string>
#include "GLShader.h"


/**
 * @brief A container used to hold all of the basic properties
 * of the GradientShader class
 */
struct GradientShaderProperties
{
		GLfloat lowColor[4];
		GLfloat highColor[4];
		GLfloat heightRange[2];

		GradientShaderProperties()
		{
			lowColor[0] = 0.0;
			lowColor[1] = 0.0;
			lowColor[2] = 0.0;
			lowColor[3] = 1.0;
			highColor[0] = 1.0;
			highColor[1] = 1.0;
			highColor[2] = 1.0;
			highColor[3] = 1.0;
			heightRange[0] = 0.0;
			heightRange[1] = 1.0;
		}
};


/**
 * @brief A two color gradient shader
 *
 * This shader draws using a gradient. The user sets low and high height values
 * as well as the colors corresponding to those values, and colors are interpolated
 * linearly within that height range. Height values below the low value are drawn
 * using the low color and height values above the high value are drawn using the
 * high value.
 *
 */
class GradientShader : public GLShader
{
	public:

		// Constructor
		GradientShader();

		// Modification Functions
		void	SetLowColor(float r, float g, float b, float a);
		void	SetHighColor(float r, float g, float b, float a);
		void	SetLowValue(float newLow);
		void	SetHighValue(float newHigh);

		// Query Functions
		GradientShaderProperties	GetShaderProperties();

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Shader Properties
		GradientShaderProperties	properties;

		// Override virtual functions
		void	CompileShader();
		void	UpdateUniforms();

};

#endif // GRADIENTSHADER_H
