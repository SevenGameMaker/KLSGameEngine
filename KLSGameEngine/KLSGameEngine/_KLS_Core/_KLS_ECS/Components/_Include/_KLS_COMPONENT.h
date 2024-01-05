// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include <yaml-cpp/yaml.h>

// unclutter the global namespace
namespace KLS
{
	struct KLS_API KLS_Component
	{
		std::string Tag = { "KLS_Component" };
		KLS_Component(const std::string& tag) : Tag(tag) {}
		virtual ~KLS_Component() {}
	};

} // end namespae