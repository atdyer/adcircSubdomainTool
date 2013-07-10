#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <QObject>
#include <string>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"

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
//		void	SetTerrainLayer(TerrainLayer* layer);
		void	SetCamera(GLCamera* cam);
		void	SetViewportSize(float w, float h);
		void	SetCenter(int newX, int newY);
		void	SetRadiusPoint(float x, float y);
		void	ScaleCircle(float scale);

		float	GetX();
		float	GetDomainX();
		float	GetY();
		float	GetDomainY();
		float	GetRadius();

	protected:

		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is being used to draw the TerrainLayer */

		// Circle attributes
		float	xPixel;
		float	xNormal;
		float	xDomain;
		float	yPixel;
		float	yNormal;
		float	yDomain;
		float	radPixel;
		float	radNormal;
		float	radDomain;
		float	zoomScale;	/**< The scaling factor for zooming */

		// Viewport attributes
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

		// Use GLU to draw the circle for now
		GLUquadricObj*	quad;	/**< The quadric object used to draw the disk */

	signals:

		void	CircleStatsSet(float, float, float);

	public slots:

		void	SetTerrainLayer(TerrainLayer *layer);
		void	CircleFinished();
};

#endif // CIRCLETOOL_H
