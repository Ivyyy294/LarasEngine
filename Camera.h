#pragma once
#include "GameObject.h"

namespace LarasEngine
{
	class Camera :
		 public GameObject
	{
	public:
		Camera ();

		virtual void Draw (QPainter* painter) override {};
		void Update () override;
		virtual size_t GetLayerId () const override { return LarasEngine::LAYER_INVISIBLE; };

	protected:
		virtual void ApplyCameraTransformation ();
	};
}


