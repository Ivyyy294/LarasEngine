#pragma once
#include "ENUM.h"
#include "GlobalDef.h"
#include "JsonObject.h"
#include "SimpleObjects.h"
#include "TimeInfo.h"
#include "Input.h"

namespace LarasEngine
{
	class GameObject
		: public SimpleGraphicObject
		, public JsonObject
	{
	public:
		GameObject (const int xPos, const int yPos, const TypId& _typId, bool loadPixmap = true);

		virtual ~GameObject () {};

		virtual void Update ();
		virtual size_t GetLayerId () const { return LayerId::LAYER_OBJECT1; };
		virtual void Draw (QPainter* painter) override;
		virtual void HandleCrash (GameObject* obj);
		virtual void Interact () {};
		virtual void Init () {};

		bool CanCollideWithObjectTyp (TypId id) const;
		inline double GetTrajectory () const { return trajectory; };
		inline bool GetCollisionOn () const { return collisionOn; };
		inline bool GetIsSolid () const { return isSolid; };
		inline int GetHitPoints () const { return hitPoints; };
		inline int GetCollisionDmg () const { return collisionDmg; };

		virtual bool CheckCrash (GameObject* obj) const;
		virtual bool CheckCrash (QRectF& _rect) const;
		virtual void ChangeHitPoints (const int damage);
		virtual void Translate (const QPointF& trans) override;
		virtual QJsonObject GetAsJsonObject() const override;
		virtual void ReadJson (const QJsonObject& json) override;

		bool CheckCrashX (GameObject* obj);
		bool CheckCrashX (const QRectF& rect);
		QRectF GetAreaOfCollision (GameObject* obj);

		typedef std::shared_ptr <GameObject> Shared;
		typedef std::vector <GameObject::Shared> Vec;
		typedef std::map <GameObject::TypId, GameObject::Vec> ObjectMap;
		typedef std::pair <GameObject*, GameObject*> Pair;
	protected:
		std::vector <TypId> collisionWhiteList;
		bool drawCollisionRect{ false };
		bool collisionOn;
		bool isSolid;
		bool checkMovementPath;
		int collisionDmg;
		int hitPoints;
		QPointF velocity;
		double trajectory;
		bool visible {true};

	private:
		bool IsMovementPathValid (const QPointF& v, QRectF& collisionRect) const;
		void DrawCollisonBox (QPainter* painter);

	};
}




