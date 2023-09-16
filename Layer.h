#pragma once
#include "SimpleObjects.h"
#include <vector>

namespace LarasEngine
{
	class Layer
	{
	public:
		Layer(void);
		~Layer(void);

		std::vector<SimpleGraphicObject*> vectorImage;

		void DrawLayer(QPainter* painter);
		void AddImage (SimpleGraphicObject* image);
		void Layer::Clear ();

		typedef std::map <size_t, Layer> Map;
	};
}


