// include the needed header files
#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "KLS_Entity.h"
#include "KLS_ECS.h"
#include "KLS_ECS_System.h"
#include "_KLS_ECS_Components.h"
#include "KLS_Level.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_Mesh.h"
#include <filesystem>
namespace fs = std::filesystem;

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_ECS::KLS_ECS(KLS_Level* level) : m_Level(level)
	{
	}

	KLS_ECS::~KLS_ECS()
	{
		removeAllSystems();
		m_Level = nullptr;
	}

	void KLS_ECS::preFrame(const double& elapsedtime)
	{
		if (!m_MarkedForDeletion.empty())
		{
			std::vector<KLS_Entity>::iterator it;
			for (it = m_MarkedForDeletion.begin(); it != m_MarkedForDeletion.end(); it++)
			{
				if ((*it).getEntity() != entt::null)
					(*it).destroy();
			}
			m_MarkedForDeletion.clear();
		}

		std::vector<KLS_ECS_System*>::iterator it;
		for (it = m_preFrameSystems.begin(); it != m_preFrameSystems.end(); it++)
		{
			if ((*it)->isActive())
				(*it)->tick(elapsedtime);
		}
	}

	void KLS_ECS::frame(const double& elapsedtime)
	{
		std::vector<KLS_ECS_System*>::iterator it;
		for (it = m_frameSystems.begin(); it != m_frameSystems.end(); it++)
		{
			if ((*it)->isActive())
				(*it)->tick(elapsedtime);
		}
	}

	KLS_Entity KLS_ECS::createEntity(KLS_UUID id, const std::string name, KLS_Transform t)
	{
		entt::entity enttEntity = m_Registry.create();
		KLS_Entity e = KLS_Entity(this, enttEntity);
			e.addComponent<KLS_COMPONENT_NAME>(name);
			e.addComponent<KLS_COMPONENT_ID>(id);
			e.addComponent<KLS_COMPONENT_CHILDREN>();
			e.addComponent<KLS_COMPONENT_CAMERA_DISTANCE>(0.0f);
			e.addComponent<KLS_COMPONENT_TRANSFORM>(t);
		return e;
	}

	void KLS_ECS::clear()
	{
		m_Registry.clear();
	}

	void KLS_ECS::removeEntity(KLS_Entity entity)
	{
		if (entity.getEntity() != entt::null)
			m_MarkedForDeletion.push_back(entity);
	}

	void KLS_ECS::addpreFrameSystem(KLS_ECS_System* system)
	{
		m_preFrameSystems.push_back(system);
	}

	void KLS_ECS::addframeSystem(KLS_ECS_System* system)
	{
		m_frameSystems.push_back(system);
	}

	void KLS_ECS::removeSystem(KLS_ECS_System* system)
	{
		std::vector<KLS_ECS_System*>::iterator it;
		for (it = m_preFrameSystems.begin(); it != m_preFrameSystems.end(); it++)
		{
			if ((*it) == system)
			{
				delete(*it);
				m_preFrameSystems.erase(it);
				return;
			}
		}
		for (it = m_frameSystems.begin(); it != m_frameSystems.end(); it++)
		{
			if ((*it) == system)
			{
				delete(*it);
				m_frameSystems.erase(it);
				return;
			}
		}
	}
	
	KLS_Entity KLS_ECS::findEntityByName(const std::string& name)
	{
		auto view = m_Registry.view<KLS_COMPONENT_NAME>();
		for (entt::entity entity : view)
		{
			auto& nameComponent = m_Registry.get<KLS_COMPONENT_NAME>(entity);
			if (nameComponent.m_Name == name)
			{
				return KLS_Entity(this, entity);
			}
		}
		return KLS_Entity(this, entt::null);
	}

	KLS_Entity KLS_ECS::findEntityById(const KLS_UUID& id)
	{
		auto view = m_Registry.view<KLS_COMPONENT_NAME>();
		for (entt::entity entity : view)
		{
			auto& idComponent = m_Registry.get<KLS_COMPONENT_ID>(entity);
			if (idComponent.Id == id)
			{
				return KLS_Entity(this, entity);
			}
		}
		return KLS_Entity(this, entt::null);
	}

	void KLS_ECS::getCount(int& count, int& inuse)
	{
		count = static_cast<int>(m_Registry.storage<entt::entity>().size());
		inuse = static_cast<int>(m_Registry.storage<entt::entity>().in_use());

	
	}

	void KLS_ECS::removeAllSystems()
	{
		std::vector<KLS_ECS_System*>::iterator it;
		for (it = m_preFrameSystems.begin(); it != m_preFrameSystems.end(); it++) { delete(*it); }
		for (it = m_frameSystems.begin(); it != m_frameSystems.end(); it++) { delete(*it); }
		m_preFrameSystems.clear();
		m_frameSystems.clear();
	}

	void KLS_ECS::onEntityDestroyed(entt::entity e)
	{
		KLS_Entity temp(this, e);

		if (temp.hasComponent<KLS_COMPONENT_MESH>())
		{
			KLS_COMPONENT_MESH& mesh = temp.getComponent<KLS_COMPONENT_MESH>();
			m_Level->getDriver()->getResourceManager()->removeMesh(mesh.m_Mesh);
		}

		if (temp.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
		{
			KLS_COMPONENT_PHYSXOBJECT& po = temp.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
			po.m_PhysxObject->cleanup();
			delete(po.m_PhysxObject);
			po.m_PhysxObject = nullptr;
		}
	}

	bool KLS_ECS::isEntityNull(entt::entity e) 
	{ 
		if (e == entt::null) return true;
		return !m_Registry.valid(e); 
	}

} // end namespace