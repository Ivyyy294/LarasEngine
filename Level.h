#pragma once
#include "Layer.h"
#include "GameObject.h"
#include "JsonObject.h"
#include "Layer.h"

namespace LarasEngine
{
	class Level
		: public JsonObject
	{
	public:
		typedef std::string LevelId;

		Level (const LevelId& id);

		virtual void Start() = 0;
		virtual void ProcessInput () = 0;
		virtual void DoPostWork() = 0;

		typedef std::shared_ptr <Level> Ptr;
		typedef std::map <LevelId, Level::Ptr> Map;

		virtual LevelId GetLevelId() const = 0;

		virtual void ReadJson (const QJsonObject& json) override;
		virtual QJsonObject GetAsJsonObject () const;
	};
}


