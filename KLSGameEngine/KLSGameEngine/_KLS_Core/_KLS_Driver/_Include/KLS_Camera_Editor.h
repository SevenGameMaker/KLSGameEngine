// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Camera.h"
#include "KLS_KeyMap.h"

// unclutter the global namespace
namespace KLS
{
	const float vMAX_ZOOM = 10.0f;
	const float vZOOM_SPEED = 1.0f;
	const float vFOV = 45.0f;

	class KLS_API KLS_Camera_Editor : public KLS_Camera
	{
	private:
		KLS_KeyMap m_KeyMap;

		KLS_VARIABLE_GET(float, MouseX, 0);
		KLS_VARIABLE_GET(float, MouseY, 0);
		KLS_VARIABLE_GET(float, LastMouseX, 0);
		KLS_VARIABLE_GET(float, LastMouseY, 0);
		KLS_VARIABLE_GET(bool, FirstMouse, true);

		KLS_VARIABLE_GET(bool, MoveForward, false);
		KLS_VARIABLE_GET(bool, MoveBackward, false);
		KLS_VARIABLE_GET(bool, MoveLeft, false);
		KLS_VARIABLE_GET(bool, MoveRight, false);

		KLS_VARIABLE_SETGET(float, MovementSpeed, 1.0f);
		KLS_VARIABLE_SETGET(float, MovementMultipler, 50.0f);
		KLS_VARIABLE_SETGET(float, MouseSensitivity, 2.50f);
		KLS_VARIABLE_SETGET(float, MaxZoom, vMAX_ZOOM);
	public:
		// class constuctor
		KLS_Camera_Editor(KLS_Driver* driver, KLS_KeyMap keymap);

		// class destructor
		virtual ~KLS_Camera_Editor();

		virtual void setActive(bool active);

		// frame emthod
		virtual void onAnimate(const double& elapsedtime);
	};

} // end namespace