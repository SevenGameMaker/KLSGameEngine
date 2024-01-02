// include this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "KLS_PhysX_Object.h"

// unclutter the golbal namespace
namespace KLS
{
	class KLS_API KLS_PhysX_Object_Static : public KLS_PhysX_Object
	{
	public:
		virtual physx::PxRigidActor* getRigidActor();

		// class variables
	private:
		physx::PxRigidStatic* m_Actor = 0;

		// getter functions
	public:
		physx::PxRigidStatic* getActor();

		// setter functions
	public:
		virtual void setActor(physx::PxRigidStatic* actor);

	public:
		// do nothing constuctor - use initialize, create and cleanup instead
		KLS_PhysX_Object_Static();

		// do nothing destuctor - use initialize, create and cleanup instead
		virtual ~KLS_PhysX_Object_Static();

		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params);

		// cleanup whatever memory mess we made (always return false from a cleanup function)
		virtual bool cleanup();

		// called before beginscene each frame
		virtual void preFrame(const float& elapsedtime);

		virtual physx::PxTransform getGlobalPose();

		virtual glm::vec3 getPosition();

		virtual glm::quat getRotation();

		virtual void setPosition(glm::vec3 pos);

		virtual void setRotation(glm::quat rot);

		virtual void getPositionAndRotation(glm::vec3& pos, glm::quat& rot);

		// add a force to the object
		virtual void addForce(glm::vec3 dir, float magnitude);

		virtual void setFreeze(bool value);

		virtual void setMass(float value);

		virtual float getMass();

		virtual void setLinearDamping(float value);

		virtual void setAngularDamping(float value);

		virtual void rotate(glm::quat rotOffset);

		virtual glm::vec3 getIn();

		virtual glm::vec3 getLeft();

		virtual glm::vec3 getUp();

		virtual void setActive(bool v);

		virtual bool isActive();

		virtual void setUserData(int id);

		virtual void setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collideswith);	// used in collision filtering 

		virtual void setIsPartOfSimulation(bool v);
	};

} // end namespace KLS
