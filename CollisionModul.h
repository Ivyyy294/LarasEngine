#pragma once

class GameObject;

namespace LarasEngine
{
	class CollisionModul
	{
	public:
		typedef std::vector <QRectF> CollisionBoxVec;

		inline void SetCollisionRect (const QRectF& r) { collisionRect = r; };
		inline void AddCollisionBox (const QRectF& r) { collisionBoxVec.push_back (r); };

		CollisionBoxVec GetCollisionBoxVec () const;
		virtual QRectF GetCollisionRect () const;

	protected:

	private:
		QRectF collisionRect;
		CollisionBoxVec collisionBoxVec;
	};
}


