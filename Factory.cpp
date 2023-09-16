#include "stdafx.h"
#include "Factory.h"

using namespace LarasEngine;

GameObject* LarasEngine::Factory::CreateObject (GameObject::TypId id)
{
	if (mapFactoryObjects.find (id) != mapFactoryObjects.end ())
		return mapFactoryObjects[id] ();

	return nullptr;
}

Level* LarasEngine::Factory::CreateLevel (Level::LevelId id)
{
	if (mapFactoryLevels.find (id) != mapFactoryLevels.end ())
		return mapFactoryLevels[id] ();

	return nullptr;
}
