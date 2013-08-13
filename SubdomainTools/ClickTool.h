#ifndef CLICKTOOL_H
#define CLICKTOOL_H

#include "adcData.h"
#include "OpenGL/GLCamera.h"
#include "Layers/TerrainLayer.h"
#include "SubdomainTools/SelectionTool.h"

class ClickTool : public SelectionTool
{
		Q_OBJECT
	public:
		ClickTool();
		~ClickTool();

		void	Draw();
};

#endif // CLICKTOOL_H
