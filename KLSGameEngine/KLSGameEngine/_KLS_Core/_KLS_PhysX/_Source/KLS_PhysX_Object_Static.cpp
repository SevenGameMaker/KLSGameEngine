// include the needed header files
#include "KLS_PhysX_Object_Static.h"
#include "KLS_Logger.h"

// unclutter the global namespace
namespace KLS
{
	physx::PxRigidActor* KLS_PhysX_Object_Static::getRigidActor()
	{
		return m_Actor;
	}

	physx::PxRigidStatic* KLS_PhysX_Object_Static::getActor()
	{
		return m_Actor;
	}

	void KLS_PhysX_Object_Static::setActor(physx::PxRigidStatic* actor)
	{
		m_Actor = actor;
	}

	// do nothing constuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Static::KLS_PhysX_Object_Static()
	{
	}

	// do nothing destuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Static::~KLS_PhysX_Object_Static()
	{
	}

	// set all variables to a known value
	void KLS_PhysX_Object_Static::initialize()
	{
		KLS_PhysX_Object::initialize();
		m_Actor = 0;
	}

	// dual creation allows for better error handling
	bool KLS_PhysX_Object_Static::create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params)
	{
		if (!KLS_PhysX_Object::create(world, params)) return false;

		return true;
	}

	// cleanup whatever memory mess we made (always return false from a cleanup function)
	bool KLS_PhysX_Object_Static::cleanup()
	{
		// safely release the actor
		if (m_Actor) { m_Actor->release(); m_Actor = 0; }

		return KLS_PhysX_Object::cleanup();
	}

	// called before beginscene each frame
	void KLS_PhysX_Object_Static::preFrame(const float& elapsedtime)
	{
		KLS_PhysX_Object::preFrame(elapsedtime);
	}

	physx::PxTransform KLS_PhysX_Object_Static::getGlobalPose()
	{
		return getActor() == nullptr ? physx::PxTransform() : getActor()->getGlobalPose();
	}

	glm::vec3 KLS_PhysX_Object_Static::getPosition()
	{
		glm::vec3 pos(0, 0, 0);
		if (getActor())
		{
			physx::PxTransform tx = getActor()->getGlobalPose();
			pos = glm::vec3(tx.p.x, tx.p.y, tx.p.z);
		}
		return pos;
	}

	glm::quat KLS_PhysX_Object_Static::getRotation()
	{
		if (getActor())
		{
			// Get the global pose (position and rotation) of the actor
			physx::PxTransform actorTransform = getActor()->getGlobalPose();

			// Convert the rotation quaternion to Euler angles (pitch, yaw, roll) in degrees
			glm::quat rotationQuaternion(actorTransform.q.w, actorTransform.q.x, actorTransform.q.y, actorTransform.q.z);
			glm::vec3 rotationAngles = glm::degrees(glm::eulerAngles(rotationQuaternion));

			// Convert the Euler angles back to a quaternion
			return glm::quat(glm::radians(rotationAngles));
		}
		return KLS_IDENTITY_QUAT;
	}

	void KLS_PhysX_Object_Static::setPosition(glm::vec3 pos)
	{
		if (getActor())
		{
			physx::PxTransform tx = getActor()->getGlobalPose();
			tx.p.x = pos.x;
			tx.p.y = pos.y;
			tx.p.z = pos.z;
			getActor()->setGlobalPose(tx);
		}
	}

	void KLS_PhysX_Object_Static::setRotation(glm::quat rot)
	{
		// Convert the glm::vec3 Euler angles to a glm::quat quaternion
		glm::quat rotationQuaternion = rot;

		// Convert the glm::quat to a PxQuat
		physx::PxQuat pxQuaternion(rotationQuaternion.x, rotationQuaternion.y, rotationQuaternion.z, rotationQuaternion.w);

		// Set the rotation of the actor using the PxQuat
		physx::PxTransform actorTransform = getActor()->getGlobalPose();
		actorTransform.q = pxQuaternion;
		getActor()->setGlobalPose(actorTransform);
	}

	void KLS_PhysX_Object_Static::getPositionAndRotation(glm::vec3& pos, glm::quat& rot)
	{
		if (getActor())
		{
			// Get the global pose (position and rotation) of the actor
			physx::PxTransform actorTransform = getActor()->getGlobalPose();

			// Extract the position as a glm::vec3
			pos = glm::vec3(actorTransform.p.x, actorTransform.p.y, actorTransform.p.z);

			// Convert the rotation quaternion to Euler angles (pitch, yaw, roll) in degrees
			glm::quat rotationQuaternion(actorTransform.q.w, actorTransform.q.x, actorTransform.q.y, actorTransform.q.z);
			glm::vec3 rotationAngles = glm::degrees(glm::eulerAngles(rotationQuaternion));

			// Convert the Euler angles back to a quaternion
			rot = glm::quat(glm::radians(rotationAngles));
		}
	}

	// add a force to the object
	void KLS_PhysX_Object_Static::addForce(glm::vec3 dir, float magnitude)
	{
		// this is a static object!
	}

	void KLS_PhysX_Object_Static::setFreeze(bool value)
	{
		// this is a static object!
	}

	void KLS_PhysX_Object_Static::setMass(float value)
	{
		// this is a static object!
	}

	float KLS_PhysX_Object_Static::getMass()
	{
		// this is a static object!
		return 0;
	}

	void KLS_PhysX_Object_Static::setLinearDamping(float value)
	{
		// this is a static object!
	}

	void KLS_PhysX_Object_Static::setAngularDamping(float value)
	{
		// this is a static object!
	}

	void KLS_PhysX_Object_Static::rotate(glm::quat rotOffset)
	{
		if (getActor())
		{
			glm::quat rot = getRotation() * rotOffset;
			setRotation(rot);
		}
	}

	glm::vec3 KLS_PhysX_Object_Static::getIn()
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 KLS_PhysX_Object_Static::getLeft()
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 KLS_PhysX_Object_Static::getUp()
	{
		return glm::vec3(0, 0, 0);
	}

	void KLS_PhysX_Object_Static::setActive(bool v)
	{
		if (getActor())
		{
			if (v) m_Actor->setName("ACTIVE");
			else m_Actor->setName("INACTIVE");
		}
	}

	bool KLS_PhysX_Object_Static::isActive()
	{
		bool value = true; // m_Actor == NULL ? false : stringc(m_Actor->getName()) == "ACTIVE";
		return value;
	}

	void KLS_PhysX_Object_Static::setUserData(int id)
	{
		if (getActor())
		{
			getActor()->userData = (void*)(intptr_t)id;
			physx::PxShape* shapes[10];
			physx::PxU32 nShapes = getActor()->getShapes(shapes, 10);
			while (nShapes--)
			{
				shapes[nShapes]->userData = (void*)(intptr_t)id;
			}
		}
	}

	void KLS_PhysX_Object_Static::setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collideswith)
	{
		if (getActor())
		{
			physx::PxShape* shapes[100];
			physx::PxU32 nShapes = getActor()->getShapes(shapes, 100);
			while (nShapes--)
			{
				physx::PxFilterData data = shapes[nShapes]->getSimulationFilterData();
				data.word0 = t;
				data.word1 = collideswith;
				shapes[nShapes]->setSimulationFilterData(data);
				shapes[nShapes]->setQueryFilterData(data);
			}
		}
	}

	void KLS_PhysX_Object_Static::setIsPartOfSimulation(bool v)
	{
		if (m_Actor)
		{
			m_Actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !v);
			/*
							PxShape* shapes[10];
							PxU32 nShapes = getActor()->getShapes(shapes, 10);
							while (nShapes--)
							{
								shapes[nShapes]->setFlag(PxShapeFlag::eSIMULATION_SHAPE, !v);
							}
			*/
		}
	}

} // end namespace