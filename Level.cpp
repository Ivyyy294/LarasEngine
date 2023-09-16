#include "stdafx.h"
#include "Level.h"
#include "GlobalDef.h"
#include "GObjectHandler.h"
#include <qjsonarray.h>

LarasEngine::Level::Level (const LevelId& id)
	: JsonObject (id)
{
}

void LarasEngine::Level::ReadJson (const QJsonObject& json)
{
	if (json.contains ("GameObjectList") && json["GameObjectList"].isArray ())
	{
		QJsonArray gobjectList = json["GameObjectList"].toArray ();

		for (int i = 0; i < gobjectList.size (); ++i)
		{
			if (gobjectList[i].isObject ())
			{
				QJsonObject gobject = gobjectList[i].toObject();

				if (gobject.contains ("TypId"))
				{
					GameObject::TypId id = gobject["TypId"].toString ().toLatin1();
					GameObject* o = LE_ADD_O (id);
					o->ReadJson (gobject);
				}
			}
		}
	}
}

QJsonObject LarasEngine::Level::GetAsJsonObject () const
{
	QJsonObject object;
	QJsonArray objectArray;

	for (const auto& typVec : LE_OH.GetMap ())
	{
		for (const auto& object : typVec.second)
			objectArray.push_back (object->GetAsJsonObject());
	}

	object.insert ("GameObjectList", objectArray);

	return object;
}

