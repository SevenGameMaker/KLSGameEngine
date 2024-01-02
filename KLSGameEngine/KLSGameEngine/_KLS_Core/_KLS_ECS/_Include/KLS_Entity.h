// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_ECS.h"
#include <entt/entt.hpp>
#include "KLS_Transform.h"

// unclutter the global namespace
namespace KLS
{
    /*
    *   container for the entt::enitty handle
    *   easily copied and transferred as needed
    *   one m_Children component is attached to each entt::entity handle
    */
    class KLS_API KLS_Entity
	{
	private:
		KLS_ECS* m_ECS{ nullptr };
        entt::entity m_Entity{ entt::null };

	public:
		KLS_ECS* getECS();
		entt::entity getEntity();
        KLS_Transform getAbsoluteTransform();

        KLS_UUID getId();
        std::string getName();
        KLS_Transform& getTransform();

    public:
        KLS_Entity() :m_ECS(nullptr), m_Entity(entt::null) {};
        KLS_Entity(KLS_ECS* ecs, entt::entity entity);
        virtual ~KLS_Entity();

        template<typename Component, typename... Args>
        Component& addComponent(Args&&... args) {
            return m_ECS->getRegistry().emplace<Component>(m_Entity, std::forward<Args>(args)...);
        }

        template<typename Component>
        void removeComponent() {
            m_ECS->getRegistry().remove<Component>(m_Entity);
        }

        template<typename Component>
        bool hasComponent() const {
            return m_ECS->getRegistry().any_of<Component>(m_Entity);
        }

        template<typename Component>
        Component& getComponent() {
            return m_ECS->getRegistry().get<Component>(m_Entity);
        }

        // simple method to determien if the entity is valid
        bool isNull() { return getECS()->isEntityNull(m_Entity); }

        // access to the children list
        std::vector<entt::entity>& getChildren();

        // destroy this entity, including all children
        void destroy();

        // parent / child relationships (core entt)
        void addChild(entt::entity child);
        void removeChild(entt::entity child);
        void destroyChild(entt::entity child);
        void setParent(entt::entity parent);

        // parent / child relationships (KLS_Entity)
        void addChild(KLS_Entity child);
        void removeChild(KLS_Entity child);
        void destroyChild(KLS_Entity child);
        void setParent(KLS_Entity parent);

        // common parent / child relationship methods
        void destroyChildren();
        void removeFromParent();
    };

} // end namespace