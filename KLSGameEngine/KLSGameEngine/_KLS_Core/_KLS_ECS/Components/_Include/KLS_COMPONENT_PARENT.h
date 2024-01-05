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
	struct KLS_API KLS_COMPONENT_PARENT : public KLS_Component
	{
		KLS_Entity Parent;

		KLS_COMPONENT_PARENT(KLS_Entity parent) :
			KLS_Component("KLS_COMPONENT_PARENT"),Parent(parent) {}
		virtual ~KLS_COMPONENT_PARENT() {}
	};


} // end namespace
