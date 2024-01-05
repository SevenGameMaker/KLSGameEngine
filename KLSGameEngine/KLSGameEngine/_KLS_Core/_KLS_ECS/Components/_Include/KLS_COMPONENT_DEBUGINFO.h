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
	struct KLS_API KLS_COMPONENT_DEBUGINFO : public KLS_Component
	{
		bool Active = true;
		KLS_Mesh* Mesh = nullptr;

		KLS_COMPONENT_DEBUGINFO(KLS_Mesh* mesh) :
			KLS_Component("KLS_COMPONENT_DEBUGINFO"),Mesh(mesh) {}
		virtual ~KLS_COMPONENT_DEBUGINFO() { Mesh = nullptr; }
	};


} // end namespace
