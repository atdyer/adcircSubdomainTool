#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <string>
#include "adcData.h"
#include "OpenGL/GLCamera.h"

/**
 * @brief This class provides functionality for cutting out a subdomain by
 * drawing a circle over the desired area of the current project
 */
class CircleTool
{
	public:
		CircleTool();
		~CircleTool();

		void	Draw();
		void	SetViewportSize(float w, float h);
		void	SetCenter(int newX, int newY);
		void	SetRadius(float newRad);

		float	GetX();
		float	GetY();
		float	GetRadius();

	protected:

		// Circle attributes
		float	x, y, rad;

		// Viewport attributes
		float	w, h, l, r, b, t;

		// Use GLU to draw the circle for now
		GLUquadricObj*	quad;
};

#endif // CIRCLETOOL_H
