#pragma once
#include "GameObject.h"
#include "GlobalDef.h"
#include <memory>

namespace LarasEngine
{
	class Background : public GameObject
	{
	public:
		Background();
		Background (Qt::GlobalColor _color);
		virtual void Draw (QPainter* painter) override;

		virtual void Update () override {};
		virtual size_t GetLayerId() const override { return LAYER_BACKGROUND; };
		typedef std::shared_ptr <Background> Ptr;

	private:
		Qt::GlobalColor color;
		bool useImage;
	};
}

