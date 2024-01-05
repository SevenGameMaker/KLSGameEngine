// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "entt/entt.hpp"
#include <KLS_ECS_YAML_Helpers.h>
#include "_KLS_Component.h"
#include "KLS_PhysX_Object.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_COMPONENT_CONTROLLER : public KLS_Component
	{
		KLS_KeyMap KeyMap;
		bool	MoveForward = false;
		bool	MoveBackward = false;
		bool	MoveTurnLeft = false;
		bool	MoveTurnRight = false;
		bool	MoveStrafeLeft = false;
		bool	MoveStrafeRight = false;
		float	MovementSpeed = 10.0f;
		float	MovementTurnSpeed = 0.1f;
		float	MovementMultipler = 50.0f;

		bool	UseMouse = false;
		float	MouseX = 0;
		float	MouseY = 0;
		float	LastMouseX = 0;
		float	LastMouseY = 0;
		bool	FirstMouse = true;
		float	MouseSensitivity = 2.50f;
		float	Pitch = 0;
		float	Yaw = 0;

		KLS_COMPONENT_CONTROLLER() :
			KLS_Component("KLS_COMPONENT_CONTROLLER") {}
		virtual ~KLS_COMPONENT_CONTROLLER() {}
	};


} // end namespace
