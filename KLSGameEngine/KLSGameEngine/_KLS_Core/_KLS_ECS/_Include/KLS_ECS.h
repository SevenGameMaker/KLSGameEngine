// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Transform.h"
#include <entt/entt.hpp>
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Level KLS_Level;
	typedef class KLS_Entity KLS_Entity;
	typedef class KLS_ECS_System KLS_ECS_System;

	struct Component
	{
		std::string name;
		std::string structure;
	};

	class KLS_API KLS_ECS
	{
	private:
		KLS_Level* m_Level = nullptr;
		entt::registry m_Registry;
		std::vector<KLS_Entity> m_MarkedForDeletion;

	public:

	public:
		entt::registry& getRegistry() { return m_Registry; }
		std::vector<KLS_ECS_System*> m_preFrameSystems;
		std::vector<KLS_ECS_System*> m_frameSystems;
		KLS_Level* getLevel() { return m_Level; };


	public:
		KLS_ECS(KLS_Level* level);
		virtual ~KLS_ECS();

		virtual void clear();

		KLS_Entity createEntity(KLS_UUID id, const std::string name, KLS_Transform t);
		void removeEntity(KLS_Entity entity);

		void preFrame(const double& elapsedtime);
		void frame(const double& elapsedtime);

		void addpreFrameSystem(KLS_ECS_System* system);
		void addframeSystem(KLS_ECS_System* system);

		void removeSystem(KLS_ECS_System* system);
		void removeAllSystems();

		KLS_Entity findEntityByName(const std::string& name);
		KLS_Entity findEntityById(const KLS_UUID& id);

		void getCount(int& count, int& inuse);

		template<typename... Component>
		auto getView() {
			return m_Registry.view<Component...>();
		}

		void onEntityDestroyed(entt::entity e);
		bool isEntityNull(entt::entity e);
	};

} // end namespace