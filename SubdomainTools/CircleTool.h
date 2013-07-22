#ifndef CIRCLETOOL_H
#define CIRCLETOOL_H

#include <QObject>
#include <string>

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"


/**
 * @brief The SelectionType enum contains the available selection states for the CircleTool. The
 * state defines what is selected when the user draws a circle.
 */
enum SelectionType {nodeSelection, elementSelection};


/**
 * @brief This class provides functionality for selecting Nodes and Elements by
 * drawing a circle over the desired area of the current TerrainLayer
 */
class CircleTool : public QObject
{
		Q_OBJECT
	public:
		CircleTool();
		~CircleTool();

		void	Draw();
		void	SetCamera(GLCamera* cam);
		void	SetViewportSize(float w, float h);
		void	SetCenter(int newX, int newY);
		void	SetRadiusPoint(int newX, int newY);

	protected:

		TerrainLayer*	terrain;	/**< The TerrainLayer that nodes/elements will be selected from */
		GLCamera*	camera;		/**< The GLCamera that is being used to draw the TerrainLayer */

		// Selection Mode
		SelectionType	selectionMode;	/**< The current selection mode of the circle tool */

		// Circle attributes
		float	xPixel;		/**< The x-coordinate of the circle center, in pixels */
		float	yPixel;		/**< The y-coordinate of the circle center, in pixels */
		float	xNormal;	/**< The x-coordinate of the circle center, in normalized OpenGL space */
		float	yNormal;	/**< The y-coordinate of the circle center, in normalized OpenGL space */
		float	xDomain;	/**< The x-coordinate of the circle center, in the original coordinate system of the TerrainLayer */
		float	yDomain;	/**< The y-coordinate of the circle center, in the original coordinate system of the TerrainLayer */

		float	edgeXPixel;	/**< The x-coordinate of the circle edge, in pixels */
		float	edgeYPixel;	/**< The y-coordinate of the circle edge, in pixels */
		float	edgeXNormal;	/**< The x-coordinate of the circle edge, in normalized OpenGL space */
		float	edgeYNormal;	/**< The y-coordinate of the circle edge, in normalized OpenGL space */
		float	edgeXDomain;	/**< The x-coordinate of the circle edge, the original coordinate system of the TerrainLayer */
		float	edgeYDomain;	/**< The y-coordinate of the circle edge, the original coordinate system of the TerrainLayer */

		float	radPixel;	/**< The radius of the circle, in pixels */
		float	radNormal;	/**< The radius of the circle, in normalized OpenGL space */
		float	radDomain;	/**< The radius of the circle, in the original coordinate system of the TerrainLayer */

		// Viewport attributes
		float	w;	/**< The viewport width */
		float	h;	/**< The viewport height */
		float	l;	/**< The left side of the viewport */
		float	r;	/**< The right side of the viewport */
		float	b;	/**< The bottom of the viewport */
		float	t;	/**< The top of the viewport */

		// Use GLU to draw the circle for now
		GLUquadricObj*	quad;	/**< The quadric object used to draw the disk */

		float	distance(float x1, float y1, float x2, float y2);

	signals:

		void	CircleStatsSet(float, float, float);
		void	CircleStatsFinished();
		void	NodeSelected(Node*);
		void	NodesSelected(std::vector<Node*>);

	public slots:

		void	SetTerrainLayer(TerrainLayer *layer);
		void	SetSelectionMode(SelectionType newMode);
		void	CircleFinished();
};

#endif // CIRCLETOOL_H
