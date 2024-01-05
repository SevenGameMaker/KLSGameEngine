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
	struct KLS_API KLS_COMPONENT_PHYSXOBJECT : public KLS_Component
	{
		KLS_PhysX_Object* PhysXObject = nullptr;

		KLS_COMPONENT_PHYSXOBJECT(KLS_PhysX_Object* po) :
			KLS_Component("KLS_COMPONENT_PHYSXOBJECT"),PhysXObject(po) {}
		virtual ~KLS_COMPONENT_PHYSXOBJECT() {}
	};


} // end namespace
