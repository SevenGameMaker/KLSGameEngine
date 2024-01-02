// include the needed header files
#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "KLS_Entity.h"
#include "KLS_ECS.h"
#include "_KLS_ECS_Components.h"

// unclutter the global namespace
namespace KLS
{
	KLS_Transform KLS_DefaultTransform;

	// class constructor / destructor (use create / cleanup instead)
	KLS_Entity::KLS_Entity(KLS_ECS* ecs, entt::entity entity) : m_ECS(ecs), m_Entity(entity)
	{
	}

	KLS_Entity::~KLS_Entity()
	{
		m_ECS = nullptr;
		m_Entity = entt::null;
	}

	std::vector<entt::entity>& KLS_Entity::getChildren()
	{
		return getComponent<KLS_COMPONENT_CHILDREN>().m_Children;
	}

	KLS_Transform KLS_Entity::getAbsoluteTransform()
	{
		KLS_COMPONENT_TRANSFORM& t = getComponent<KLS_COMPONENT_TRANSFORM>();
		KLS_Transform result = t.m_Transform;

		if (hasComponent<KLS_COMPONENT_PARENT>())
		{
			KLS_COMPONENT_PARENT parent = getComponent<KLS_COMPONENT_PARENT>();
			KLS_Transform pt = parent.m_Parent.getAbsoluteTransform();
			result += pt;
		}

		return result;
	}

	KLS_UUID KLS_Entity::getId()
	{
		if (isNull()) return 0;
		KLS_COMPONENT_ID& id = getComponent<KLS_COMPONENT_ID>();
		return id.Id;
	}

	std::string KLS_Entity::getName()
	{
		if (isNull()) return "NULL";
		KLS_COMPONENT_NAME& name = getComponent<KLS_COMPONENT_NAME>();
		return name.m_Name;
	}

	KLS_Transform& KLS_Entity::getTransform()
	{
		if (isNull()) return KLS_DefaultTransform;
		return getComponent<KLS_COMPONENT_TRANSFORM>().m_Transform;
	}

	KLS_ECS* KLS_Entity::getECS()
	{
		return m_ECS;
	}

	entt::entity KLS_Entity::getEntity()
	{
		return m_Entity;
	}

	void KLS_Entity::addChild(entt::entity child)
	{
		// make it a KLS_Entity
		KLS_Entity theChild(m_ECS, child);

		// Check if the child already has a parent
		if (theChild.hasComponent<KLS_COMPONENT_PARENT>())
		{
			// Remove child from its existing parent
			KLS_COMPONENT_PARENT parentComponent = theChild.getComponent<KLS_COMPONENT_PARENT>();
			parentComponent.m_Parent.removeChild(child);
		}

		// Add the child to the m_Children vector
		getChildren().push_back(child);

		// Add the KLS_COMPONENT_PARENT component to the child
		theChild.addComponent<KLS_COMPONENT_PARENT>(KLS_Entity(getECS(), m_Entity));
	}

	void KLS_Entity::removeChild(entt::entity child)
	{
		// Make it a KLS_Entity
		KLS_Entity theChild(m_ECS, child);

		// Check if the child has a parent
		if (theChild.hasComponent<KLS_COMPONENT_PARENT>())
		{
			// Get the parent component of the child
			KLS_COMPONENT_PARENT parentComponent = theChild.getComponent<KLS_COMPONENT_PARENT>();

			if (!getChildren().empty())
			{
				std::vector<entt::entity>::iterator it;
				for (it = getChildren().begin(); it != getChildren().end(); it++)
				{
					if ((*it) == child)
					{
						getChildren().erase(it);
						break;
					}
				}
			}

			// Remove the KLS_COMPONENT_PARENT component from the child
			theChild.removeComponent<KLS_COMPONENT_PARENT>();
		}
	}

	void KLS_Entity::setParent(entt::entity parent)
	{
		KLS_Entity p(getECS(), parent);
		p.addChild(m_Entity);
	}

	void KLS_Entity::removeFromParent()
	{
		// Check if the child has a parent
		if (hasComponent<KLS_COMPONENT_PARENT>())
		{
			// Get the parent component of the child
			KLS_COMPONENT_PARENT parentComponent = getComponent<KLS_COMPONENT_PARENT>();
			KLS_Entity p(getECS(), parentComponent.m_Parent.getEntity());
			p.removeChild(m_Entity);
		}
	}

	void KLS_Entity::addChild(KLS_Entity child)
	{
		addChild(child.getEntity());
	}

	void KLS_Entity::removeChild(KLS_Entity child)
	{
		removeChild(child.getEntity());
	}

	void KLS_Entity::destroyChild(KLS_Entity child)
	{
		destroyChild(child.getEntity());
	}

	void KLS_Entity::setParent(KLS_Entity parent)
	{
		setParent(parent.getEntity());
	}

	void KLS_Entity::destroyChild(entt::entity child)
	{
		KLS_Entity temp(getECS(), child);
		temp.destroy();
	}

	void KLS_Entity::destroyChildren()
	{
		if (!getChildren().empty())
		{
			std::vector<entt::entity>::iterator it;
			for (it = getChildren().begin(); it != getChildren().end();)
			{
				KLS_Entity temp(getECS(), (*it));
				temp.destroyChildren();

				KLS_COMPONENT_ID& id = temp.getComponent<KLS_COMPONENT_ID>();
				KLS_UUID  tempid = id.Id;

				// let the ECS cleanup whatever is attached to the entity
				getECS()->onEntityDestroyed((*it));

				// Remove this entity from the registry
				m_ECS->getRegistry().destroy((*it));

				// Remove the current child from this entity's children
				it = getChildren().erase(it);
			}
		}
	}

	void KLS_Entity::destroy()
	{
		removeFromParent();
		destroyChildren();

		KLS_COMPONENT_ID& id = getComponent<KLS_COMPONENT_ID>();
		KLS_UUID  tempid = id.Id;

		// let the ECS cleanup whatever is attached to the entity
		getECS()->onEntityDestroyed(m_Entity);

		// Remove this entity from the registry
		m_ECS->getRegistry().destroy(m_Entity);

		m_Entity = entt::null;
	}

} // end namespace