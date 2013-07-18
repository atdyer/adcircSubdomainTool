#ifndef LAYER_H
#define LAYER_H

#include <QObject>

#include "OpenGL/GLCamera.h"
#include "OpenGL/Shaders/GLShader.h"
#include "OpenGL/Shaders/SolidShader.h"
#include "OpenGL/Shaders/GradientShader.h"

/**
 * @brief A generic wrapper class for ADCIRC layers
 *
 * This virtual class guarantees that every layer generated has a unique
 * identifier and that each layer can be drawn by the OpenGL context.
 *
 * Layers are subclasses of QObject to allow for multithreading, which can be very
 * useful when reading large data files.
 *
 * Threading usage:
 * - Create the new Layer object (using new)
 * - Create a new QThread object (using new)
 * - Move the new Layer to the QThread using Layer::moveToThread()
 * - Connect the Layer's finishedThreading signal to the QThread's deleteLater slot
 * - Use the Layer as normal, being sure to delete it when finished. This will also
 *   delete the QThread that the Layer has been using once all work on that thread
 *   has finished.
 *
 * What this does is allow all slots of the Layer object to be processed on a
 * worker thread that is alive for the lifetime of the Layer itself, preventing the
 * main GUI from freezing during long operations like reading files.
 *
 */
class Layer : public QObject
{
		Q_OBJECT
	public:

		// Constructor
		Layer(QObject* parent = 0);
		~Layer();

		// Load to GPU method
		virtual void	LoadDataToGPU() = 0;

		// Draw method
		virtual void	Draw() = 0;

		// Getter Methods
		unsigned int	GetID();
		unsigned int	GetNumLayers();
		SolidShaderProperties		GetSolidOutline();
		SolidShaderProperties		GetSolidFill();
		GradientShaderProperties	GetGradientOutline();
		GradientShaderProperties	GetGradientFill();

		// Setter Methods
		void		SetCamera(GLCamera* newCamera);


	protected:

		GLShader*	outlineShader;		/**< Pointer to the GLShader object that should be used to draw the outline */
		GLShader*	fillShader;		/**< Pointer to the GLShader object that should be used to draw the fill */

	private:

		GLCamera*	camera;			/**< The camera used for drawing in all of this Layer's shaders */
		SolidShader*	solidOutline;		/**< Shader used to draw a solid outline */
		SolidShader*	solidFill;		/**< Shader used to draw a solid fill */
		GradientShader*	gradientOutline;	/**< Shader used to draw a gradient outline */
		GradientShader*	gradientFill;		/**< Shader used to draw a gradient fill */

		static unsigned int	layerCount;	/**< A running count of the number of Layer objects */
		static unsigned int	nextID;		/**< The next available layerID */
		unsigned int		layerID;	/**< A unique unsigned integer that identifies this Layer */

	public slots:

		// Solid Shader Methods
		void	UseSolidOutlineShader(float r, float g, float b, float a);
		void	UseSolidFillShader(float r, float g, float b, float a);

		// Gradient Shader Methods
		void	UseGradientOutlineShader(float lowColor[], float highColor[], float lowVal, float highVal);
		void	UseGradientFillShader(float lowColor[], float highColor[], float lowVal, float highVal);

	signals:

		void	emitMessage(QString);
		void	startedReadingData();
		void	progress(int);
		void	finishedReadingData();
		void	finishedLoadingToGPU();
};

#endif // LAYER_H
