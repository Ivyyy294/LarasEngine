#pragma once
#include "Level.h"
#include "GameObject.h"

namespace LarasEngine
{
	class Factory
	{
	public:
		using CreateFuncObject = GameObject * (*) ();
		inline void RegisterCreateFuncObjects (GameObject::TypId id, CreateFuncObject func) { mapFactoryObjects[id] = func; };

		using CreateFuncLevel = Level * (*) ();
		inline void RegisterCreateFuncLevels (Level::LevelId id, CreateFuncLevel func) { mapFactoryLevels[id] = func; };
	
	protected:
		std::map <CoreObject::TypId, CreateFuncObject> mapFactoryObjects;
		std::map <Level::LevelId, CreateFuncLevel> mapFactoryLevels;

		GameObject* CreateObject (GameObject::TypId id);
		Level* CreateLevel (Level::LevelId id);
	};
}

