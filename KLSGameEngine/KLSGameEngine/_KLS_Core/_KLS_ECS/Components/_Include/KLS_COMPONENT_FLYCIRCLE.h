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
	struct KLS_API KLS_COMPONENT_FLYCIRCLE : public KLS_Component
	{
		glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);
		float Radius = 10.0f;
		float Speed = 1.0f;

		glm::vec3 Direction = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 VecU = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 VecV = glm::vec3(0.0f, 0.0f, 0.0f);
		float RadiusEllipsoid = 0.0f;
		float StartTime = 0.0f;
		bool First = true;

		KLS_COMPONENT_FLYCIRCLE() :
			KLS_Component("KLS_COMPONENT_FLYCIRCLE") {}
		virtual ~KLS_COMPONENT_FLYCIRCLE() {}
	};


} // end namespace
