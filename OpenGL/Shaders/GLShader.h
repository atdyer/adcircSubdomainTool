#ifndef GLSHADER_H
#define GLSHADER_H

#include "OpenGL/GLCamera.h"



/**
 * @brief A virtual class that provides the functionality needed to use
 * a GLSL shader to draw on the OpenGL context
 *
 * This class provides a wrapper that allows us to use any type of shader
 * before performing a drawing operation. It is up to the programmer to
 * ensure the correct shaders are used when drawing different Layers by
 * using a LayerManager.
 *
 */
class GLShader
{
	public:

		// Constructor/Destructor
		GLShader();
		~GLShader();

		// Function Definitions
		void	Use();
		void	SetCamera(GLCamera *newCamera);

		// Getter Methods
		unsigned int	GetID();
		unsigned int	GetNumShaders();

	protected:

		// Variable Definitions
		GLuint		programID;
		GLCamera	*camera;

		// Flags
		bool	loaded;
		bool	camSet;
		bool	uniformsSet;

		GLuint		CompileShaderPart(const char *source, GLenum shaderType);

		/**
		 * @brief Compiles the full shader program
		 *
		 * This function, defined in a subclass of GLShader, compiles all parts of the shader
		 * program for use in the OpenGL context. Upon completion of compiling/linking, the
		 * programID value should be set.
		 *
		 */
		virtual void	CompileShader() = 0;

		/**
		 * @brief Updates the shader uniforms in the OpenGL context
		 *
		 * This function, defined in a subclass of GLShader, transfers all appropriate values
		 * from the shader object to the shader program in the OpenGL context using the
		 * glGetUniformLocation() and glUniform*() functions.
		 *
		 */
		virtual void	UpdateUniforms() = 0;


	private:

		static unsigned int	shaderCount;	/**< A running count of the number of Shader objects */
		static unsigned int	nextID;		/**< The next available shaderID */
		unsigned int		shaderID;	/**< A unique unsigned integer that identifies this Layer */

};

#endif // GLSHADER_H
