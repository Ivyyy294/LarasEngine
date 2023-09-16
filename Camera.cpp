#include "stdafx.h"
#include "Camera.h"
#include "Input.h"
#include "GObjectHandler.h"

LarasEngine::Camera::Camera ()
	: GameObject (0, 0, TYPID_CAMERA, false)
{
}

void LarasEngine::Camera::Update ()
{
	ApplyCameraTransformation ();
}

void LarasEngine::Camera::ApplyCameraTransformation ()
{
	if (Input::Me ().TestKey (Input::Key::Key_W))
		LE_OH.GetCameraTransform ().translate (0.f, -1.f);
}
