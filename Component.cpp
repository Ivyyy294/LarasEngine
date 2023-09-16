#include "stdafx.h"
#include "GameObject.h"
#include "Component.h"

LarasEngine::Component::Component (GameObject* _gameObject)
	: gameObject (_gameObject)
{
}
