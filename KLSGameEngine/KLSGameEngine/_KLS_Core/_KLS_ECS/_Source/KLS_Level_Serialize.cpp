// include the needed header files
#include "KLS_Level.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"
#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_UniformData.h"
#include "KLS_PhysX_World.h"
#include <fstream>
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"

// unclutter the global namespace
namespace KLS
{
	void KLS_Level::clear()
	{
		getECS()->clear();
	}

	void KLS_Level::serializeEntity(YAML::Emitter& out, KLS_Entity entity)
	{
	}

	bool KLS_Level::saveToDisk(std::string filepath)
	{
		return true;
	}


	void KLS_Level::deserializeEntity(YAML::Node& data, KLS_Entity entity)
	{
	}

	bool KLS_Level::loadFromDisk(std::string filepath)
	{
		return true;
	}

} // end namespace