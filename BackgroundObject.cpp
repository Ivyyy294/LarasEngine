#include "stdafx.h"
#include "BackgroundObject.h"

using namespace LarasEngine;

Background::Background()
	: GameObject (0, 0, TYPID_BACKGROUND)
	, useImage (true)
{
	Resize (screenW);
	SetPosition (0, 0);
}

LarasEngine::Background::Background (Qt::GlobalColor _color)
	: GameObject (0, 0, TYPID_BACKGROUND, false)
	, color (_color)
	, useImage (false)
{
}

void LarasEngine::Background::Draw (QPainter* painter)
{
	if (useImage)
	{
		QRectF rect = GetImageRect ();
		painter->drawPixmap (rect, GetImage(), rect);
	}
	else
	{
		QRectF r (0.f, 0.f, screenW, screenH);
		painter->fillRect (r, QBrush (color));
	}
}


