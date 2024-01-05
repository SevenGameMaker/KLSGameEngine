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
	struct KLS_API KLS_COMPONENT_ROTATE : public KLS_Component
	{
		glm::vec3 m_RotateSpeed = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_RotationDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		double m_TotalTime = 0.0f;

		KLS_COMPONENT_ROTATE() :
			KLS_Component("KLS_COMPONENT_ROTATE") {}
		virtual ~KLS_COMPONENT_ROTATE() {}
	};


} // end namespace
