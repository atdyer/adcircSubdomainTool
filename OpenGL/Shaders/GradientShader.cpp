#include "GradientShader.h"


/**
 * @brief Constructor that defines the shader source code and default color and height values
 */
GradientShader::GradientShader()
{
	vertexSource =	"#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform vec4 LowColor;"
			"uniform vec4 HighColor;"
			"uniform vec2 HeightRange;"
			"void main(void)"
			"{"
			"	float range = HeightRange[1]-HeightRange[0];"
			"	if (range != 0.0)"
			"	{"
			"		float percentage = ((in_Position.z - HeightRange[0])/range);"
			"		ex_Color.r = LowColor.r + percentage*(HighColor.r - LowColor.r);"
			"		ex_Color.g = LowColor.g + percentage*(HighColor.g - LowColor.g);"
			"		ex_Color.b = LowColor.b + percentage*(HighColor.b - LowColor.b);"
			"		ex_Color.a = LowColor.a + percentage*(HighColor.a - LowColor.a);"
			"	} else {"
			"		ex_Color = LowColor;"
			"	}"
			"	gl_Position = MVPMatrix*in_Position;"
			"}";

	vertexSource =	"#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform vec4 LowColor;"
			"uniform vec4 HighColor;"
			"uniform vec2 HeightRange;"
			"void main(void)"
			"{"
			"	ex_Color = mix(LowColor, HighColor, smoothstep(HeightRange[0], HeightRange[1], in_Position.z));"
			"	gl_Position = MVPMatrix*in_Position;"
			"}";

	vertexSource =	"#version 330"
			"\n"
			"layout(location=0) in vec4 in_Position;"
			"out vec4 ex_Color;"
			"uniform mat4 MVPMatrix;"
			"uniform int stopCount;"
			"uniform float values[10];"
			"uniform vec4 colors[10];"
			"void main(void)"
			"{"
			"	ex_Color = colors[0];"
			"	for (int i=1; i<min(stopCount, 10); ++i)"
			"	{"
			"		ex_Color = mix(ex_Color, colors[i], smoothstep(values[i-1], values[i], in_Position.z));"
			"	}"
			"	gl_Position = MVPMatrix*in_Position;"
			"}";

	fragSource =	"#version 330"
			"\n"
			"in vec4 ex_Color;"
			"out vec4 out_Color;"
			"void main(void)"
			"{"
			"	out_Color = ex_Color;"
			"}";

	properties.lowColor[0] = 1.0;
	properties.lowColor[1] = 1.0;
	properties.lowColor[2] = 1.0;
	properties.lowColor[3] = 1.0;

	properties.highColor[0] = 1.0;
	properties.highColor[1] = 1.0;
	properties.highColor[2] = 1.0;
	properties.highColor[3] = 1.0;

	properties.heightRange[0] = 0.0;
	properties.heightRange[1] = 1.0;

	CompileShader();
	UpdateUniforms();
}


/**
 * @brief Set the color corresponding to the low height value
 *
 * Sets the color that corresponds to the low height value. Color values
 * are defined in the range [0.0 - 1.0].
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @param a Alpha value
 */
void GradientShader::SetLowColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		properties.lowColor[0] = r;
	if (g >= 0.0)
		properties.lowColor[1] = g;
	if (b >= 0.0)
		properties.lowColor[2] = b;
	if (a >= 0.0)
		properties.lowColor[3] = a;
}


/**
 * @brief Set the color corresponding to the high height value
 *
 * Sets the color that corresponds to the high height value. Color values
 * are defined in the range [0.0 - 1.0]
 *
 * @param r Red value
 * @param g Green value
 * @param b Blue value
 * @param a Alpha value
 */
void GradientShader::SetHighColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		properties.highColor[0] = r;
	if (g >= 0.0)
		properties.highColor[1] = g;
	if (b >= 0.0)
		properties.highColor[2] = b;
	if (a >= 0.0)
		properties.highColor[3] = a;
}


/**
 * @brief Set the low height value
 *
 * Sets the low height value that is used to define the bottom of the color gradient
 *
 * @param newLow The new value
 */
void GradientShader::SetLowValue(float newLow)
{
	properties.heightRange[0] = newLow;
}


/**
 * @brief Set the high height value
 *
 * Sets the high height value that is used to define the top of the color gradient
 *
 * @param newHigh The new value
 */
void GradientShader::SetHighValue(float newHigh)
{
	properties.heightRange[1] = newHigh;
}


/**
 * @brief Retrieves the shader's properties
 *
 * Retrieves the shader's properties, which include color and data range.
 *
 * @return The shader's properties
 */
GradientShaderProperties GradientShader::GetShaderProperties()
{
	return properties;
}


/**
 * @brief Compiles the shader parts and assembles them into a usable shader on the OpenGL context
 */
void GradientShader::CompileShader()
{
	const char* fullVertSource = vertexSource.data();
	const char* fullFragSource = fragSource.data();

	GLuint vertexShaderID = CompileShaderPart(fullVertSource, GL_VERTEX_SHADER);
	GLuint fragmentShaderID = CompileShaderPart(fullFragSource, GL_FRAGMENT_SHADER);

	if (vertexShaderID && fragmentShaderID)
	{
		programID = glCreateProgram();
		glAttachShader(programID, vertexShaderID);
		glAttachShader(programID, fragmentShaderID);
		glLinkProgram(programID);
		glDeleteShader(vertexShaderID);
		glDeleteShader(fragmentShaderID);
		loaded = true;
	}
}


/**
 * @brief Updates values used for drawing
 *
 * This function updates the MVP matrix as well as color and height range values used in
 * drawing operations.
 *
 */
void GradientShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint StopCountUniform = glGetUniformLocation(programID, "stopCount");
		GLint ValuesUniform = glGetUniformLocation(programID, "values");
		GLint ColorsUniform = glGetUniformLocation(programID, "colors");

		GLfloat colors[8];
		for (int i=0; i<4; ++i)
		{
			colors[i] = properties.lowColor[i];
			colors[4+i] = properties.highColor[i];
		}

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform1i(StopCountUniform, 2);
		glUniform1fv(ValuesUniform, 2, properties.heightRange);
		glUniform4fv(ColorsUniform, 2, colors);

		GLenum errVal = glGetError();
		if (errVal != GL_NO_ERROR)
		{
			const GLubyte *errString = gluErrorString(errVal);
			DEBUG("GradientShader OpenGL Error: " << errString);
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
