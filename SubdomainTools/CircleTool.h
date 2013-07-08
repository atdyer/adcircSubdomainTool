#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <QObject>

#include <string>
#include "adcData.h"
#include "OpenGL/GLCamera.h"

/**
 * @brief This class provides functionality for cutting out a subdomain by
 * drawing a circle over the desired area of the current project
 */
class CircleTool : public QObject
{
		Q_OBJECT
	public:
		CircleTool();
		~CircleTool();

		void	Draw();
		void	SetViewportSize(float w, float h);
		void	SetCenter(int newX, int newY);
		void	SetDomainCenter(float newX, float newY);
		void	SetRadius(float newRad);

		float	GetX();
		float	GetDomainX();
		float	GetY();
		float	GetDomainY();
		float	GetRadius();

	protected:

		// Circle attributes
		float	x, domainX, y, domainY, rad;

		// Viewport attributes
		float	w, h, l, r, b, t;

		// Use GLU to draw the circle for now
		GLUquadricObj*	quad;

	signals:

		void	CircleStatsSet(float, float, float);
};

#endif // CIRCLETOOL_H
