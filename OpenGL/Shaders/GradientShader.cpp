#include "GradientShader.h"

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

	fragSource =	"#version 330"
			"\n"
			"in vec4 ex_Color;"
			"out vec4 out_Color;"
			"void main(void)"
			"{"
			"	out_Color = ex_Color;"
			"}";

	lowColor[0] = 1.0;
	lowColor[1] = 1.0;
	lowColor[2] = 1.0;
	lowColor[3] = 1.0;

	highColor[0] = 1.0;
	highColor[1] = 1.0;
	highColor[2] = 1.0;
	highColor[3] = 1.0;

	heightRange[0] = 0.0;
	heightRange[1] = 1.0;

	CompileShader();
	UpdateUniforms();
}


void GradientShader::SetLowColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		lowColor[0] = r;
	if (g >= 0.0)
		lowColor[1] = g;
	if (b >= 0.0)
		lowColor[2] = b;
	if (a >= 0.0)
		lowColor[3] = a;
}


void GradientShader::SetHighColor(float r, float g, float b, float a)
{
	if (r >= 0.0)
		highColor[0] = r;
	if (g >= 0.0)
		highColor[1] = g;
	if (b >= 0.0)
		highColor[2] = b;
	if (a >= 0.0)
		highColor[3] = a;
}


void GradientShader::SetLowValue(float newLow)
{
	heightRange[0] = newLow;
}


void GradientShader::SetHighValue(float newHigh)
{
	heightRange[1] = newHigh;
}


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


void GradientShader::UpdateUniforms()
{
	if (loaded && camSet)
	{
		glUseProgram(programID);

		GLint MVPUniform = glGetUniformLocation(programID, "MVPMatrix");
		GLint LowUniform = glGetUniformLocation(programID, "LowColor");
		GLint HighUniform = glGetUniformLocation(programID, "HighColor");
		GLint HeightUniform = glGetUniformLocation(programID, "HeightRange");

		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, camera->MVPMatrix.m);
		glUniform4fv(LowUniform, 1, lowColor);
		glUniform4fv(HighUniform, 1, highColor);
		glUniform2fv(HeightUniform, 1, heightRange);

		GLenum errVal = glGetError();
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
