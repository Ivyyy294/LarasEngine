#include "stdafx.h"
#include "Layer.h"

using namespace LarasEngine;

Layer::Layer(void)
{
}


Layer::~Layer(void)
{
}

void Layer::DrawLayer(QPainter* painter)
{
	for (auto i : vectorImage)
	{
		if (i != nullptr)
			i->Draw(painter);
	}
}

void Layer::AddImage (SimpleGraphicObject* image)
{
	vectorImage.push_back (image);
}

void Layer::Clear ()
{
	vectorImage.clear();
}