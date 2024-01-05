// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "entt/entt.hpp"
#include <KLS_ECS_YAML_Helpers.h>
#include "_KLS_Component.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_COMPONENT_SKYBOX : public KLS_Component
	{
		KLS_Mesh* Mesh = nullptr;

		KLS_COMPONENT_SKYBOX(KLS_Mesh* mesh) :
			KLS_Component("KLS_COMPONENT_SKYBOX"),Mesh(mesh) {}
		virtual ~KLS_COMPONENT_SKYBOX() {}
	};


} // end namespace
