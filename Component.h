#pragma once
#include <vector>
#include <qjsonobject.h>

namespace LarasEngine
{
	class GameObject;

	class Component
	{
	public:
		typedef std::shared_ptr<Component> Ptr;
		typedef std::vector<Component::Ptr> Vec;
		typedef std::string ComponentId;

		virtual void Start (const QJsonObject& json) = 0;
		virtual void Update (const size_t& playerInput, const double lag) {};
		virtual void Interact () {};
		virtual void HandleCrash (GameObject* obj) {};

	protected:
		Component (GameObject* _gameObject);
		
		GameObject* gameObject;
	};
}


