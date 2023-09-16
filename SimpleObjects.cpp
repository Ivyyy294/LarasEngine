#include "stdafx.h"
#include "SimpleObjects.h"
#include "GObjectHandler.h"

using namespace LarasEngine;


SimpleTextObject::SimpleTextObject (const int xPos, const int yPos, const QString _text, const CoreObject::TypId& _typId)
	: CoreObject (_typId)
	, text (_text)
{
	SetPosition (xPos, yPos);
}

void SimpleTextObject::SetFont (const QFont& _font)
{
	font = _font;
	text.prepare (QTransform (), font);
}

void SimpleTextObject::SetText (const QString& _text)
{
	text.setText (_text);
	text.prepare (QTransform (), font);
}

void SimpleTextObject::Draw (QPainter* painter)
{
	painter->save ();
	painter->setFont (font);
	painter->setPen (pen);
	painter->drawStaticText (GetPosition(), text);
	painter->restore ();
}

SimpleGraphicObject::SimpleGraphicObject (const int xPos, const int yPos, const CoreObject::TypId& _typId, bool loadPixmap)
	: CoreObject (_typId)
	, indexImage (0)

{
	SetPosition (xPos, yPos);

	if (loadPixmap)
		LoadImage (typId);

	SetCollisionRect (image.rect ());
}

void SimpleGraphicObject::Draw (QPainter* painter)
{
	painter->save ();

	if (rotation != 0.0)
		ApplyRotation (painter);

	QTransform trans = painter->transform () * LE_OH.GetCameraTransform ();
	painter->setTransform (trans);

	QRectF tmpRect = rect;
	tmpRect.moveTopLeft (GetPosition ());
	painter->drawPixmap (tmpRect, image, image.rect ());

	painter->restore ();
}

QRectF LarasEngine::SimpleGraphicObject::GetCollisionRect () const
{
	QRectF tmpRect = CoreObject::GetCollisionRect ();
	tmpRect.moveCenter (GetCenter ());
	return tmpRect;
}

void SimpleGraphicObject::SetScale (double scale)
{
	image = image.scaledToWidth (image.width () * scale, Qt::TransformationMode::SmoothTransformation);
	rect = image.rect ();
}

QRectF LarasEngine::SimpleGraphicObject::GetImageRect () const
{
	QRectF tmp = rect;
	QPointF pos = GetPosition ();
	tmp.moveTopLeft (pos);
	return tmp;
}

void LarasEngine::SimpleGraphicObject::MoveCenter (const QPointF& p)
{
	QRectF tmp = rect;
	tmp.moveCenter (p);
	SetPosition (tmp.topLeft());
}

void SimpleGraphicObject::Resize (size_t length)
{
	const QPointF center = rect.center ();
	rect.setWidth (length);
	rect.setHeight (length);
	SetCollisionRect (rect);

	image = image.scaledToWidth (length);
}

void LarasEngine::SimpleGraphicObject::LoadImage (const std::string& imagePath)
{
	image = LE_OH.LoadImage (imagePath);
	rect = image.rect ();
}

void SimpleGraphicObject::AddImage (const QString& imagePath)
{
	imageVec.push_back (QImage (imagePath));
}

bool SimpleGraphicObject::GetImageAtPos (const int pos, QImage& image)
{
	if (pos >= 0 && pos < imageVec.size ())
	{
		image = imageVec.at (pos);
		return true;
	}

	return false;
}

void SimpleGraphicObject::SwitchImage (const TypId& id)
{
	image = LE_OH.LoadImage (id).scaledToWidth (image.width ());
	rect = image.rect ();
}

void LarasEngine::SimpleGraphicObject::ApplyRotation (QPainter* painter)
{
	QRectF tmpRect = GetImageRect ();
	double xc = tmpRect.topLeft ().x () + image.width () / 2;
	double yc = tmpRect.topLeft ().y () + image.height () / 2;

	painter->translate (xc, yc);
	painter->rotate (rotation);
	painter->translate (-xc, -yc);
}
