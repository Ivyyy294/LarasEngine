#pragma once

#include "GameObject.h"

namespace LarasEngine
{
	class Projectile : public GameObject
	{
	public:
		Projectile (int x, int y, double r, TypId id);

		void Update () override;

		void SetLaunchVelocity (QPointF p);
	protected:
		QPointF launchVelocity;
		double lifetime;
		qint64 lifetimeMs{ 0 };
		QLineF path;
		virtual void ReadJson (const QJsonObject& json) override;
	};
}