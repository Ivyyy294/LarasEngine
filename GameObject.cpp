#include "stdafx.h"
#include "GameObject.h"
#include "GObjectHandler.h"
#include "GlobalDef.h"
#include <qjsonarray.h>

using namespace LarasEngine;

void GameObject::Translate (const QPointF& _point)
{
	if (checkMovementPath)
	{
		for (char i = 0; i < 2; ++i)
		{
			//i == 0 X Movement
			//i == 1 Y Movement
			bool xMovement = i == 0;

			QRectF collision;

			QPointF v = xMovement ? QPointF (_point.x(), 0.f) : QPointF (0.f, _point.y());

			if (v.manhattanLength() > 0.f && !IsMovementPathValid (v, collision))
			{
				QRectF rect = GetImageRect ();

				if (xMovement)
				{
					if (collision.x () > rect.x ())
						v.setX (v.x () - collision.width ());
					else
						v.setX (v.x () + collision.width ());
				}
				else
				{
					if (collision.y () > rect.y ())
						v.setY (v.y () - collision.height ());
					else			 
						v.setY (v.y () + collision.height ());
				}

				CoreObject::Translate (v);
			}
			else
				CoreObject::Translate (v);
		}
	}
	else
		CoreObject::Translate (_point);
}

bool LarasEngine::GameObject::CanCollideWithObjectTyp (TypId id) const
{
	if (collisionWhiteList.size () == 0)
		return true;

	return std::find (collisionWhiteList.begin(), collisionWhiteList.end(), id) == collisionWhiteList.end();
}

GameObject::GameObject (const int xPos, const int yPos, const CoreObject::TypId& _typId, bool loadPixmap)
	: SimpleGraphicObject (xPos, yPos, _typId, loadPixmap)
	, JsonObject (typId)
	, hitPoints (0)
	, collisionOn (false)
	, collisionDmg (0)
	, isSolid (false)
	, checkMovementPath (false)
{
#ifdef _DEBUG
	drawCollisionRect = true;
#endif // DEBUG

}

void GameObject::Update ()
{
	if (!velocity.isNull ())
		Translate (velocity * TimeInfo::Me ().GetFactor ());
}

void LarasEngine::GameObject::Draw (QPainter* painter)
{
	if (visible)
		SimpleGraphicObject::Draw (painter);

	if (drawCollisionRect && collisionOn)
		DrawCollisonBox (painter);
}

void GameObject::HandleCrash (GameObject* obj)
{
	ChangeHitPoints (obj->GetCollisionDmg ());

	if (hitPoints <= 0)
		deleteMe = true;
}

bool GameObject::CheckCrash (GameObject* obj) const
{
	CollisionBoxVec objColVec = obj->GetCollisionBoxVec ();

	if (!objColVec.empty ())
	{
		for (auto i : objColVec)
		{
			if (CheckCrash (i))
				return true;
		}

		return false;
	}
	else
		return CheckCrash (obj->GetCollisionRect());
}

bool GameObject::CheckCrash (QRectF& _rect) const
{
	CollisionBoxVec colVec = GetCollisionBoxVec ();

	if (!colVec.empty ())
	{
		for (auto i : colVec)
		{
			if (i.intersects (_rect))
				return true;
		}

		return false;
	}
	else
		return GetCollisionRect().intersects (_rect);
}

void GameObject::ChangeHitPoints (const int damage)
{
	hitPoints -= damage;
}

void GameObject::ReadJson (const QJsonObject& json)
{
	if (json.contains ("hitPoints"))
		hitPoints = json["hitPoints"].toInt ();

	if (json.contains ("collisionDmg"))
		collisionDmg = json["collisionDmg"].toInt ();

	if (json.contains ("collisionOn"))
		collisionOn = json["collisionOn"].toBool ();

	if (json.contains ("velocity") && json["velocity"].isArray ())
	{
		QJsonArray velocityArray = json["velocity"].toArray ();
		velocity.setX (velocityArray[0].toDouble ());
		velocity.setY (velocityArray[1].toDouble ());
	}

	if (json.contains ("isSolid"))
		isSolid = json["isSolid"].toBool ();

	if (json.contains ("Position") && json["Position"].isArray ())
	{
		QJsonArray positionArray = json["Position"].toArray ();
		QPointF pos (positionArray.at (0).toDouble (), positionArray.at (1).toDouble ());
		
		SetPosition (pos);
	}

	if (json.contains ("CollisionBox") && json["CollisionBox"].isArray ())
	{
		QJsonArray ColBoxArray = json["CollisionBox"].toArray ();

		for (int i = 0; i < ColBoxArray.size (); ++i)
		{
			if (ColBoxArray[i].isObject ())
			{
				QJsonObject gobject = ColBoxArray[i].toObject ();

				if (gobject.contains ("Size")
					&& gobject.contains ("Position"))
				{
					QJsonArray sizeArray = gobject["Size"].toArray ();
					QJsonArray posArray = gobject["Position"].toArray ();
					
					QRectF r;
					r.setWidth (sizeArray[0].toInt ());
					r.setHeight (sizeArray[1].toInt ());
					r.translate (QPointF (posArray[0].toInt(), posArray[1].toInt()));

					AddCollisionBox (r);
				}
			}
		}
	}
}

QJsonObject LarasEngine::GameObject::GetAsJsonObject () const
{
	QJsonObject object;
	object.insert ("TypId", typId.c_str());
	object.insert ("hitPoints", hitPoints);
	object.insert ("collisionDmg", collisionDmg);
	object.insert ("collisionOn", collisionOn);

	QJsonArray velocityArray;
	velocityArray.push_back (velocity.x ());
	velocityArray.push_back (velocity.y ());

	object.insert ("velocity", velocityArray);

	object.insert ("isSolid", isSolid);

	QJsonArray positionArray;
	QPointF pos = GetPosition ();
	positionArray.push_back (pos.x ());
	positionArray.push_back (pos.y());

	object.insert ("Position", positionArray);

	return object;
}

bool LarasEngine::GameObject::IsMovementPathValid (const QPointF& v, QRectF& collisionRect) const
{
	QRectF rectTmp = GetImageRect();
	QPointF NewPos = rectTmp.center () + v;
	rectTmp.moveCenter (NewPos);

	const auto& map = LE_OH.GetMap ();

	for (const auto& i : map)
	{
		for (const auto& j : i.second)
		{
			if (j.get() != this && j->isSolid && j->CheckCrash (rectTmp))
			{
				collisionRect = j->GetCollisionRect().intersected (rectTmp);
				return false;
			}
		}
	}

	return true;
}

void LarasEngine::GameObject::DrawCollisonBox (QPainter* painter)
{
	painter->save ();
	painter->setTransform (LE_OH.GetCameraTransform ());

	painter->setPen (Qt::red);

	CollisionBoxVec colBoxVec = GetCollisionBoxVec ();

	if (!colBoxVec.empty ())
	{
		for (const auto& i : colBoxVec)
			painter->drawRect (i);
	}
	else
		painter->drawRect (GetCollisionRect ());

	painter->restore ();
}

bool GameObject::CheckCrashX (GameObject* obj)
{
	QRectF collisionRect = obj->GetCollisionRect ();
	QRectF rect2 (collisionRect.x (), 0, collisionRect.width (), 1);
	return CheckCrashX (rect2);
}

bool GameObject::CheckCrashX (const QRectF& rect2)
{
	QRectF collisionRect = GetCollisionRect ();
	QRectF rect1 (collisionRect.x (), 0, collisionRect.width (), 1);
	bool ok = rect1.intersects (rect2);

	return ok;
}

QRectF GameObject::GetAreaOfCollision (GameObject* obj)
{
	QRectF collisionRect = GetCollisionRect ();

	if (collisionRect.isNull())
		return GetCollisionRect().intersected (obj->GetCollisionRect ());
	else
		return collisionRect.intersected (obj->GetCollisionRect ());
}
