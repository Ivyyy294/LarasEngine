#include "stdafx.h"
#include "Input.h"

using namespace LarasEngine;

Input& Input::Me ()
{
	static Input me;
	return me;
}

void LarasEngine::Input::SetKey (Input::Key key, bool active)
{
	playerInput[key] = active;
}

bool LarasEngine::Input::TestKey (Input::Key key) const
{
	if (playerInput.find (key) != playerInput.end ())
		return playerInput.at (key);

	return false;
}
