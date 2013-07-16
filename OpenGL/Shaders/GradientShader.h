#ifndef GRADIENTSHADER_H
#define GRADIENTSHADER_H

#include <string>
#include "GLShader.h"


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

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Color variables
		GLfloat	lowColor[4];
		GLfloat	highColor[4];

		// Data variables
		GLfloat	heightRange[2];

		// Override virtual functions
		void	CompileShader();
		void	UpdateUniforms();

};

#endif // GRADIENTSHADER_H
