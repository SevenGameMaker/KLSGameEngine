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
	struct KLS_API KLS_COMPONENT_HILITE : public KLS_Component
	{
		int MeshBuffer = -1;

		KLS_COMPONENT_HILITE(int meshbuffer) :
			KLS_Component("KLS_COMPONENT_HILITE"),MeshBuffer(meshbuffer) {}
		virtual ~KLS_COMPONENT_HILITE() {}
	};


} // end namespace
