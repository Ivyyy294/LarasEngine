#pragma once
#include "CoreObject.h"
#include <qjsonobject.h>
#include <vector>

namespace LarasEngine
{
	class Script
	{
	public:
		typedef std::shared_ptr<Script> Ptr;
		typedef std::vector <Script::Ptr> Vec;

		Script (CoreObject* _gameObject);
		virtual void Start (const QJsonObject& json) = 0;
		virtual void Update (const double lag) {};
		virtual void Interact () {};

	protected:
		CoreObject* gameObject{ nullptr };
	};
}


