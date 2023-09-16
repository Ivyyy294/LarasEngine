#include "stdafx.h"
#include "DebugInfoObject.h"
#include "GObjectHandler.h"

using namespace LarasEngine;

DebugInfoObject::DebugInfoObject ()
	: GameObject (0, 0, TYPID_DEBUG_INFO, false)
{
	textRect.setWidth (screenW / 4);
	textRect.setHeight (screenH / 2);
	font = QApplication::font ();
	font.setPointSize (12);
}

void DebugInfoObject::Draw (QPainter* painter)
{
	painter->save ();
	painter->setPen (Qt::red);
	painter->setFont (font);

	QRectF r = textRect;
	r.translate (GetPosition ());
	painter->drawText (r, Qt::AlignLeft, text);
	painter->restore ();

	GameObject::Draw (painter);
}

void DebugInfoObject::Update ()
{
	text = GetFpsAsString ();
	text += GetObjectVecAsString ();
}

QString DebugInfoObject::GetFpsAsString () const
{
	QString text;
	qint64 frameTime = TimeInfo::Me ().GetFrameTime ();
	text += "Frametime: " + QString::number (frameTime) + "\n";
	text += "FPS: " + QString::number (std::floor (1000.f / frameTime)) + "\n";
	return text;
}

QString DebugInfoObject::GetObjectVecAsString () const
{
	QString text;

	for (const auto& i : LE_OH.GetMap ())
		text += QString::fromStdString (i.first) + ": " + QString::number (i.second.size()) + "\n";

	return text;
}

