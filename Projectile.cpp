#include "stdafx.h"
#include "Projectile.h"

using namespace LarasEngine;

Projectile::Projectile (int x, int y, double r, TypId id)
	: GameObject (x, y, id)
	, lifetime (0.0)
{
	path.setP1 (QPointF (0.f, 0.f));
	SetRotation (std::floor (r));
}

void Projectile::Update ()
{
	double lag = TimeInfo::Me ().GetFactor ();
	lifetimeMs += TimeInfo::Me().GetFrameTime ();
	lifetime += lag;

	Translate (path.p2() * lag);

	if (!launchVelocity.isNull ())
		Translate (launchVelocity * lag);

	QRectF rect = GetImageRect ();
	deleteMe = (rect.y () <= 0 && rect.y () >= screenH);
}

void LarasEngine::Projectile::SetLaunchVelocity (QPointF p)
{
	launchVelocity = p;
	//Translate (p * TimeInfo::Me ().GetFactor ());
}

void LarasEngine::Projectile::ReadJson (const QJsonObject& json)
{
	GameObject::ReadJson (json);
	path.setP2 (velocity);
	path.setAngle (path.angle () - rotation);
}

