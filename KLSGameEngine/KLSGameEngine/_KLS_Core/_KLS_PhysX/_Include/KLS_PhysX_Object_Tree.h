// include this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "KLS_PhysX_Object.h"
#include "KLS_PhysX_Object_Dynamic.h"
#include "KLS_PhysX_Object_Static.h"

// unclutter the golbal namespace
namespace KLS
{
	#define MAX_ACTORS 100
	class KLS_API KLS_PhysX_Object_Tree : public KLS_PhysX_Object_Static
	{
		// class variables
	private:
		physx::PxRigidStatic* m_Actors[MAX_ACTORS];

		// getter functions
	public:

		// setter functions
	public:

	public:
		// do nothing constuctor - use initialize, create and cleanup instead
		KLS_PhysX_Object_Tree() {}

		// do nothing destuctor - use initialize, create and cleanup instead
		virtual ~KLS_PhysX_Object_Tree() {}

		// set all variables to a known value
		virtual void initialize()
		{
			KLS_PhysX_Object_Static::initialize();

			// set to a known value
			for (int x = 0; x < MAX_ACTORS; x++) m_Actors[x] = 0;
		}

		// dual creation allows for better error handling
		virtual bool create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params)
		{
			if (!KLS_PhysX_Object_Static::create(world, params)) return false;

			return true;
		}

		// cleanup whatever memory mess we made (always return false from a cleanup function)
		virtual bool cleanup()
		{
			// safely release the actors
			for (int x = 1; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x]) { m_Actors[x]->release(); m_Actors[x] = 0; }
			}

			return KLS_PhysX_Object_Static::cleanup();
		}

		virtual void setPosition(glm::vec3 pos)
		{
			KLS_PhysX_Object_Static::setPosition(pos);

			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					physx::PxTransform tx = getActor()->getGlobalPose();
					tx.p.x = pos.x;
					tx.p.y = pos.y;
					tx.p.z = pos.z;
					m_Actors[x]->setGlobalPose(tx);
				}
			}
		}

		virtual void setRotation(glm::quat rot)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					// Convert the glm::vec3 Euler angles to a glm::quat quaternion
					glm::quat rotationQuaternion = rot;

					// Convert the glm::quat to a PxQuat
					physx::PxQuat pxQuaternion(rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z, rotationQuaternion.w);

					// Set the rotation of the actor using the PxQuat
					physx::PxTransform actorTransform = getActor()->getGlobalPose();
					actorTransform.q = pxQuaternion;
					m_Actors[x]->setGlobalPose(actorTransform);
				}
			}
		}

		void addActor(physx::PxRigidStatic* actor)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (!m_Actors[x])
				{
					m_Actors[x] = actor;
					if (x == 0)
					{
						setActor(m_Actors[0]);
					}
					return;
				}
			}
		}

		virtual void setUserData(int id)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					m_Actors[x]->userData = (void*)(intptr_t)id;
					physx::PxShape* shapes[10];
					physx::PxU32 nShapes = m_Actors[x]->getShapes(shapes, 10);
					while (nShapes--)
					{
						shapes[nShapes]->userData = (void*)(intptr_t)id;
					}
				}
			}
		}

		virtual void setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collideswith)	// used in collision filtering 
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					physx::PxShape* shapes[10];
					physx::PxU32 nShapes = m_Actors[x]->getShapes(shapes, 10);
					while (nShapes--)
					{
						physx::PxFilterData data = shapes[nShapes]->getSimulationFilterData();
						data.word0 = t;
						data.word1 = collideswith;
						shapes[nShapes]->setSimulationFilterData(data);
						shapes[nShapes]->setQueryFilterData(data);
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
					}
				}
			}
		}

		virtual void setActive(bool v)
		{
			if (m_Actors[0])
			{
				if (v) m_Actors[0]->setName("ACTIVE");
				else m_Actors[0]->setName("INACTIVE");
			}
		}

		virtual bool isActive()
		{
			bool value = true; // m_Actors[0] == NULL ? false : stringc(m_Actors[0]->getName()) == "ACTIVE";
			return value;
		}

	};

	class KLS_PhysX_Object_TreeDynamic : public KLS_PhysX_Object_Dynamic
	{
		// class variables
	private:
		physx::PxRigidDynamic* m_Actors[MAX_ACTORS];

		// getter functions
	public:

		// setter functions
	public:

	public:
		// do nothing constuctor - use initialize, create and cleanup instead
		KLS_PhysX_Object_TreeDynamic() {}

		// do nothing destuctor - use initialize, create and cleanup instead
		virtual ~KLS_PhysX_Object_TreeDynamic() {}

		// set all variables to a known value
		virtual void initialize()
		{
			KLS_PhysX_Object_Dynamic::initialize();

			// set to a known value
			for (int x = 0; x < MAX_ACTORS; x++) m_Actors[x] = 0;
		}

		// dual creation allows for better error handling
		virtual bool create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params)
		{
			if (!KLS_PhysX_Object_Dynamic::create(world, params)) return false;

			return true;
		}

		// cleanup whatever memory mess we made (always return false from a cleanup function)
		virtual bool cleanup()
		{
			// safely release the actors
			for (int x = 1; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x]) { m_Actors[x]->release(); m_Actors[x] = 0; }
			}

			return KLS_PhysX_Object_Dynamic::cleanup();
		}

		virtual void setPosition(glm::vec3 pos)
		{
			KLS_PhysX_Object_Dynamic::setPosition(pos);
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					physx::PxTransform tx = getActor()->getGlobalPose();
					tx.p.x = pos.x;
					tx.p.y = pos.y;
					tx.p.z = pos.z;
					m_Actors[x]->setGlobalPose(tx);
				}
			}
		}

		virtual void setRotation(glm::quat rot)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					// Convert the glm::vec3 Euler angles to a glm::quat quaternion
					glm::quat rotationQuaternion = rot;

					// Convert the glm::quat to a PxQuat
					physx::PxQuat pxQuaternion(rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z, rotationQuaternion.w);

					// Set the rotation of the actor using the PxQuat
					physx::PxTransform actorTransform = getActor()->getGlobalPose();
					actorTransform.q = pxQuaternion;
					m_Actors[x]->setGlobalPose(actorTransform);
				}
			}
		}

		void addActor(physx::PxRigidDynamic* actor)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (!m_Actors[x])
				{
					m_Actors[x] = actor;
					if (x == 0)
					{
						setActor(m_Actors[0]);
					}
					return;
				}
			}
		}

		virtual void setUserData(int id)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					m_Actors[x]->userData = (void*)(intptr_t)id;
					physx::PxShape* shapes[10];
					physx::PxU32 nShapes = m_Actors[x]->getShapes(shapes, 10);
					while (nShapes--)
					{
						shapes[nShapes]->userData = (void*)(intptr_t)id;
					}
				}
			}
		}

		virtual void setObjectType(long t)
		{
			for (int x = 0; x < MAX_ACTORS; x++)
			{
				if (m_Actors[x])
				{
					physx::PxShape* shapes[10];
					physx::PxU32 nShapes = m_Actors[x]->getShapes(shapes, 10);
					while (nShapes--)
					{
						physx::PxFilterData data = shapes[nShapes]->getSimulationFilterData();
						data.word0 = t;
						shapes[nShapes]->setSimulationFilterData(data);
						shapes[nShapes]->setQueryFilterData(data);
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eSCENE_QUERY_SHAPE, true);
					}
				}
			}
		}
		virtual void setActive(bool v)
		{
			if (m_Actors[0])
			{
				if (v) m_Actors[0]->setName("ACTIVE");
				else m_Actors[0]->setName("INACTIVE");
			}
		}

		virtual bool isActive()
		{
			bool value = true; // m_Actors[0] == NULL ? false : stringc(m_Actors[0]->getName()) == "ACTIVE";
			return value;
		}
	};

} // end namespace KLS
