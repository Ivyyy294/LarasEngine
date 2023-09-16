#pragma once
#include "ENUM.h"
#include "Event.h"

#define PI 3.14159265
#define LE_OH LarasEngine::GObjectHandler::Me()
#define GAMESTATUS LE_OH.GetGameStatus()
#define LE_ADD_E LE_OH.AddEvent
#define LE_ADD_O LE_OH.AddObject

#define TYPID_BACKGROUND "BACKGROUND"
#define TYPID_DIALOG "DIALOG"
#define TYPID_CAMERA "CAMERA"
#define TYPID_DEBUG_INFO "DEBUG_INFO"

namespace LarasEngine
{
	enum LayerId
	{
		LAYER_BACKGROUND,
		LAYER_OBJECT1,
		LAYER_OBJECT2,
		LAYER_HUD,
		LAYER_DIALOG,
		LAYER_INVISIBLE,
		LAYER_END
	};
}
