// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "entt/entt.hpp"
#include <KLS_ECS_YAML_Helpers.h>
#include "_KLS_Component.h"
#include "KLS_Camera.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_COMPONENT_CAMERA : public KLS_Component
	{
		KLS_Camera* Camera = nullptr;

		KLS_COMPONENT_CAMERA(KLS_Camera* cam) :
			KLS_Component("KLS_COMPONENT_CAMERA"), Camera(cam) {}
		virtual ~KLS_COMPONENT_CAMERA() {}
	};


} // end namespace
