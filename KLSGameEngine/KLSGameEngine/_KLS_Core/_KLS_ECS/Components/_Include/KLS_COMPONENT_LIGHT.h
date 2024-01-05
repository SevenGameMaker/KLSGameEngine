// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "entt/entt.hpp"
#include <KLS_ECS_YAML_Helpers.h>
#include "_KLS_Component.h"
#include "KLS_UniformData.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_COMPONENT_LIGHT : public KLS_Component
	{
		KLS_LightData LightData;

		KLS_COMPONENT_LIGHT(KLS_LightData l) :
			KLS_Component("KLS_COMPONENT_LIGHT"), LightData(l) {}
		virtual ~KLS_COMPONENT_LIGHT() {}
	};


} // end namespace
