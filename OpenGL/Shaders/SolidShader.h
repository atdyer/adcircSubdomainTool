#ifndef SOLIDSHADER_H
#define SOLIDSHADER_H

#include <string>
#include "GLShader.h"


/**
 * @brief A container used to hold all of the basic properties
 * of the SolidShader class
 */
struct SolidShaderProperties
{
		GLfloat color[4];

		SolidShaderProperties()
		{
			color[0] = 0.0;
			color[1] = 0.0;
			color[2] = 0.0;
			color[3] = 1.0;
		}

		SolidShaderProperties(float r, float g, float b, float a)
		{
			color[0] = r;
			color[1] = g;
			color[2] = b;
			color[3] = a;
		}
};


/**
 * @brief A single color shader
 *
 * This shader draws everything the same solid color.
 *
 */
class SolidShader : public GLShader
{
	public:

		// Constructor
		SolidShader();

		// Modification Functions
		void	SetColor(float r, float g, float b, float a);

		// Query Functions
		SolidShaderProperties	GetShaderProperties();

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Shader Properties
		SolidShaderProperties	properties;

		// Override virtual functions
		void	CompileShader();
		void	UpdateUniforms();
};

#endif // SOLIDSHADER_H
