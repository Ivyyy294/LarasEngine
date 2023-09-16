#include "stdafx.h"
#include "CoreObject.h"
#include "GObjectHandler.h"

using namespace LarasEngine;

LarasEngine::CoreObjectSimple::CoreObjectSimple (const std::string& _typId)
	: typId (_typId)
{
}

double CoreObject::CalculateXViaRotation (int s, double r)
{
	double tanA = tan (std::fabs (r) * PI / 180);
	return tanA * std::fabs (s);
}

CoreObject::CoreObject (const TypId& _typId)
	: deleteMe (false)
	, CoreObjectSimple (_typId)
	, rotation (0.f)
{
	const QRect& screenGeo = LE_OH.GetPrimaryScreenGeometry ();
	screenW = screenGeo.width ();
	screenH = screenGeo.height ();
	position.setX (0.f);
	position.setY (0.f);
}

QRectF LarasEngine::CoreObject::GetCollisionRect () const
{
	QRectF tmp = collisionRect;
	tmp.moveTopLeft (position);
	return tmp;
}

std::vector<QRectF> LarasEngine::CoreObject::GetCollisionBoxVec () const
{
	std::vector<QRectF> vec;

	for (auto i : collisionBoxVec)
	{
		i.translate (position);
		vec.push_back (i);
	}

	return vec;
}
