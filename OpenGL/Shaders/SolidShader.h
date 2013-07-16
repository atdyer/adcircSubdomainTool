#ifndef SOLIDSHADER_H
#define SOLIDSHADER_H

#include <string>
#include "GLShader.h"


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

	protected:

		// Source code
		std::string	vertexSource;
		std::string	fragSource;

		// Color variables
		GLfloat	rgba[4];

		// Override virtual functions
		void	CompileShader();
		void	UpdateUniforms();
};

#endif // SOLIDSHADER_H
