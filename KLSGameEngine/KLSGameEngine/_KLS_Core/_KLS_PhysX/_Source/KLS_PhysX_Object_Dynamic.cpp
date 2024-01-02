// include the needed header files
#include "KLS_PhysX_Object_Dynamic.h"
#include "KLS_Logger.h"

// unclutter the golbal namespace
namespace KLS
{
	physx::PxRigidActor* KLS_PhysX_Object_Dynamic::getRigidActor() 
	{ 
		return m_Actor; 
	}
	
	physx::PxRigidDynamic* KLS_PhysX_Object_Dynamic::getActor() 
	{ 
		return m_Actor; 
	}

	void KLS_PhysX_Object_Dynamic::setActor(physx::PxRigidDynamic* actor) 
	{ 
		m_Actor = actor; 
		setActive(true); 
	}

	// do nothing constuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Dynamic::KLS_PhysX_Object_Dynamic() 
	{
	}

	// do nothing destuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Dynamic::~KLS_PhysX_Object_Dynamic() 
	{
	}

	// set all variables to a known value
	void KLS_PhysX_Object_Dynamic::initialize()
	{
		KLS_PhysX_Object::initialize();
		m_Actor = 0;
	}

	// dual creation allows for better error handling
	bool KLS_PhysX_Object_Dynamic::create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params)
	{
		if (!KLS_PhysX_Object::create(world, params)) return false;

		return true;
	}

	// cleanup whatever memory mess we made (always return false from a cleanup function)
	bool KLS_PhysX_Object_Dynamic::cleanup()
	{
		// safely release the actor
		if (m_Actor) { m_Actor->release(); m_Actor = 0; }

		return KLS_PhysX_Object::cleanup();
	}

	// called before beginscene each frame
	void KLS_PhysX_Object_Dynamic::preFrame(const float& elapsedtime)
	{
		KLS_PhysX_Object::preFrame(elapsedtime);
	}

	physx::PxTransform KLS_PhysX_Object_Dynamic::getGlobalPose()
	{
		return getActor() == nullptr ? physx::PxTransform() : getActor()->getGlobalPose();
	}

	glm::vec3 KLS_PhysX_Object_Dynamic::getPosition()
	{
		glm::vec3 pos(0, 0, 0);
		if (getActor())
		{
			physx::PxTransform tx = getActor()->getGlobalPose();
			pos = glm::vec3(tx.p.x, tx.p.y, tx.p.z);
		}
		return pos;
	}

	glm::quat KLS_PhysX_Object_Dynamic::getRotation()
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

	void KLS_PhysX_Object_Dynamic::setPosition(glm::vec3 pos)
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

	void KLS_PhysX_Object_Dynamic::setRotation(glm::quat rot)
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

	void KLS_PhysX_Object_Dynamic::getPositionAndRotation(glm::vec3& pos, glm::quat& rot)
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
	void KLS_PhysX_Object_Dynamic::addForce(glm::vec3 dir, float magnitude)
	{
		if ((getActor()) && (magnitude > 0))
		{
			glm::vec3 v = dir * magnitude;
			physx::PxVec3 f(v.x, v.y, v.z);
			getActor()->setLinearVelocity(f);
		}
	}

	void KLS_PhysX_Object_Dynamic::setFreeze(bool value)
	{
		if (getActor())
		{
			if (!value) getActor()->wakeUp();
			else getActor()->putToSleep();
		}
	}

	void KLS_PhysX_Object_Dynamic::setMass(float value)
	{
		if (getActor())
		{
			getActor()->setMass(value);
			// since we set the mass we need to update this
			physx::PxRigidBodyExt::updateMassAndInertia(*m_Actor, value);
		}
	}

	float KLS_PhysX_Object_Dynamic::getMass()
	{
		if (getActor())	return getActor()->getMass();
		return 0;
	}

	void KLS_PhysX_Object_Dynamic::setLinearDamping(float value)
	{
		if (getActor())	getActor()->setLinearDamping(value);
	}

	void KLS_PhysX_Object_Dynamic::setAngularDamping(float value)
	{
		if (getActor())	getActor()->setAngularDamping(value);
	}

	void KLS_PhysX_Object_Dynamic::rotate(glm::quat rotOffset)
	{
		if (getActor())
		{
			glm::quat rot = getRotation() * rotOffset;
			setRotation(rot);
		}
	}

	glm::vec3 KLS_PhysX_Object_Dynamic::getIn()
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 KLS_PhysX_Object_Dynamic::getLeft()
	{
		return glm::vec3(0, 0, 0);
	}

	glm::vec3 KLS_PhysX_Object_Dynamic::getUp()
	{
		return glm::vec3(0, 0, 0);
	}

	void KLS_PhysX_Object_Dynamic::setActive(bool v)
	{
		if (getActor())
		{
			if (v) m_Actor->setName("ACTIVE");
			else m_Actor->setName("INACTIVE");
		}
	}

	bool KLS_PhysX_Object_Dynamic::isActive()
	{
		bool value = true; // m_Actor == NULL ? false : std::string(m_Actor->getName()) == "ACTIVE";
		return value;
	}

	void KLS_PhysX_Object_Dynamic::setUserData(int id)
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

	void KLS_PhysX_Object_Dynamic::setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collideswith)
	{
		if (getActor())
		{
			physx::PxShape* shapes[10];
			physx::PxU32 nShapes = getActor()->getShapes(shapes, 10);
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

	void KLS_PhysX_Object_Dynamic::setIsPartOfSimulation(bool v)
	{
		if (m_Actor)
		{
			m_Actor->setActorFlag(physx::PxActorFlag::eDISABLE_SIMULATION, !v);
			m_Actor->wakeUp();
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

	void KLS_PhysX_Object_Dynamic::setKenematic(bool v)
	{
		KLS_PhysX_Object::setKenematic(v);
		getActor()->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, v);
		getActor()->wakeUp();
	}

	bool KLS_PhysX_Object_Dynamic::isSleeping()
	{
		return m_Actor == NULL ? false : m_Actor->isSleeping();
	}

} // end namespace KLS
