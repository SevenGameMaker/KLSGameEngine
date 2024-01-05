// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "entt/entt.hpp"
#include <KLS_ECS_YAML_Helpers.h>
#include "_KLS_Component.h"
#include "KLS_Transform.h"

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_COMPONENT_INFO : public KLS_Component
	{
		std::string					Tag = { "KLS_Component" };
		int							Id = { -1 };
		std::string					Name = { "KLS_Component" };
		KLS_Transform				Transform;
		std::vector<entt::entity>	Children;
		float						CameraDistance = { 0.0f };

		KLS_COMPONENT_INFO(int id, const std::string& name,KLS_Transform t) : 
			KLS_Component("KLS_COMPONENT_INFO"),Id(id),Name(name),Transform(t),CameraDistance(0) {}
		virtual ~KLS_COMPONENT_INFO() {}
	};


} // end namespace
