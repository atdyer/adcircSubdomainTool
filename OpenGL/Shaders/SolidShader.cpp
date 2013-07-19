#include "SolidShader.h"


/**
 * @brief Constructor that defines the source code and default color values
 */
SolidShader::SolidShader()
{
	vertexSource =  "#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform vec4 ColorVector;"
			"void main(void)"
			"{"
			"       gl_Position = MVPMatrix*in_Position;"
			"       ex_Color = ColorVector;"
			"}";

	fragSource =	"#version 330"
			"\n"
			"in vec4 geo_Color;"
			"out vec4 out_Color;"
			"void main(void)"
			"{"
			"	out_Color = geo_Color;"
			"}";

	geoSource =	"#version 330"
			"\n"
			"layout (triangles) in;"
			"layout (triangle_strip, max_vertices=3) out;"
			"in vec4 ex_Color[];"
			"out vec4 geo_Color;"
			"void main(void)"
			"{"
			"	float d1 = distance(gl_in[0].gl_Position, gl_in[1].gl_Position);"
			"	float d2 = distance(gl_in[1].gl_Position, gl_in[2].gl_Position);"
			"	float d3 = distance(gl_in[0].gl_Position, gl_in[2].gl_Position);"
			"	if ((d1+d2+d3)/3.0 > 0.001)"
			"	{"
			"		for (int i=0; i<gl_in.length(); i++)"
			"		{"
			"			geo_Color = ex_Color[i];"
			"			gl_Position = gl_in[i].gl_Position;"
			"			EmitVertex();"
			"		}"
			"	}"
			"}";

	properties.color[0] = 1.0;
	properties.color[1] = 1.0;
	properties.color[2] = 1.0;
	properties.color[3] = 1.0;

	CompileShader();
	UpdateUniforms();
}


/**
 * @brief Set the color to be used in glDraw*() operations
 *
 * Pass a negative value to any individual color value to keep it the same
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @param a Alpha value
 */
void SolidShader::SetColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		properties.color[0] = r;
	if (g >= 0.0)
		properties.color[1] = g;
	if (b >= 0.0)
		properties.color[2] = b;
	if (a >= 0.0)
		properties.color[3] = a;
}


/**
 * @brief Retrieves the shader's properties
 *
 * Retrieves the shader's properties (ie. color).
 *
 * @return The shader's properties
 */
SolidShaderProperties SolidShader::GetShaderProperties()
{
	return properties;
}


/**
 * @brief Compiles the shader parts and assembles them into a usable shader on the OpenGL context
 */
void SolidShader::CompileShader()
{
	const char* fullVertSource = vertexSource.data();
	const char* fullFragSource = fragSource.data();
	const char* fullGeoSource = geoSource.data();

	GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);
	GLuint geoShaderID = CompileShaderPart(fullGeoSource, GL_GEOMETRY_SHADER);

	if (vertexShaderID && fragmentShaderID && geoShaderID)
	{
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glAttachShader(programID, geoShaderID);
		glLinkProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		glDeleteShader(geoShaderID);
		loaded = true;
	}
}


/**
 * @brief Updates values used for drawing
 *
 * This function updates the MVP matrix as well as the color used in drawing operations.
 *
 */
void SolidShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint ColorUniform = glGetUniformLocation(programID, "ColorVector");

		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
			DEBUG("Error getting uniform locations");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(ColorUniform, 1, properties.color);

		errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("SolidShader OpenGL Error: " << errString);
			uniformsSet = false;
		} else {
			uniformsSet = true;
		}

	} else {
		if (!loaded)
			DEBUG("Uniforms not updated: Shader not loaded");
		else
			DEBUG("Uniforms not updated: Camera not set");
		uniformsSet = false;
	}
}
