#pragma once
#include "GameObject.h"

class DebugInfoObject :
	public LarasEngine::GameObject
{
public:
	DebugInfoObject ();

	virtual void Draw (QPainter* painter) override;
	virtual void Update () override;

	static GameObject* CreateMethod () { return new DebugInfoObject; };
	virtual size_t GetLayerId () const override { return LarasEngine::LAYER_HUD; };
private:
	QRectF textRect;
	QString text;
	QFont font;

	QString GetFpsAsString () const;
	QString GetObjectVecAsString () const;
};

