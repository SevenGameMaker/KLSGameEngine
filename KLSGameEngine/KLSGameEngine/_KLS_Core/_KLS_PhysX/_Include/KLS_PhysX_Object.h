// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include <PxPhysicsAPI.h>
#include "KLS_Logger.h"
#include <KLS_PhysX_ObjectCreationParams.h>

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_PhysX_World KLS_PhysX_World;

	/*
	* Irrlicht engine wrapper
	*/
	class KLS_API KLS_PhysX_Object
	{
		// class variables
	private:
		KLS_VARIABLE_GET(KLS_PhysX_World*, World, nullptr);
		KLS_VARIABLE_SETGET(bool, IsTrigger, false);
		KLS_VARIABLE_GET(KLS_PhysX_ObjectCreationParams, Params, KLS_PhysX_ObjectCreationParams());

	public:
		// do nothing constuctor - use initialize, create and cleanup instead
		KLS_PhysX_Object();

		// do nothing destuctor - use initialize, create and cleanup instead
		virtual ~KLS_PhysX_Object();

		// set all variables to a known value
		virtual void initialize();

		// dual creation allows for better error handling
		virtual bool create(KLS_PhysX_World* world, KLS_PhysX_ObjectCreationParams params);

		// cleanup whatever memory mess we made (always return false from a cleanup function)
		virtual bool cleanup();

		// called before beginscene each frame
		virtual void preFrame(const float& elapsedtime);

		// exposed class methods
	public:
		virtual glm::vec3				getPosition()	{ return glm::vec3(0, 0, 0);	}
		virtual glm::quat				getRotation()	{ return glm::vec3(0, 0, 0);	}
		virtual float					getMass()		{ return 0;						}
		virtual glm::vec3				getIn()			{ return glm::vec3(0, 0, 0);	}
		virtual glm::vec3				getLeft()		{ return glm::vec3(0, 0, 0);	}
		virtual glm::vec3				getUp()			{ return glm::vec3(0, 0, 0);	}
		virtual physx::PxRigidActor*	getRigidActor() { return nullptr;				}

		virtual void setPosition(glm::vec3 pos)		{	}	// set the objects position
		virtual void setRotation(glm::quat rot)		{	}	// set the obejcts rotation
		virtual void setFreeze(bool value)			{	}	// set whether the object is frozen or not (frozen objects do not get simulation action)
		virtual void setMass(float value)			{	}	// set the mass of the object (more like density but whatever :)
		virtual void setLinearDamping(float value)	{	}	// how much to slow the object each frame
		virtual void setAngularDamping(float value) {	}	// how much to stop the object from spinning each frame
		virtual void setUserData(int id)			{	}	// set the userdata normally this is the id of the container object
		virtual void setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collidswith) {	}	// used in collision filtering 
		virtual void setIsPartOfSimulation(bool v)	{	}	// add or remove from the simulation
		virtual void setKenematic(bool v)			{	}	// toggle this as kenematic (kenematic objects are not moved by simulation)
		virtual void setActive(bool v)				{	}	// set actor name to ACTIVE or INACTIVE

		// rotate the object relative to it's current rotation
		virtual void rotate(glm::quat rotOffset);

		// add a force to the object
		virtual void addForce(glm::vec3 dir, float magnitude) { KLS_WARN("base object add force"); }

		// is the object active in the simulation
		virtual bool isActive() { return true; }

		// is the object sleeping
		virtual bool isSleeping() { return false; }

		// get both the positiona dn the rotation of the obejct
		virtual void getPositionAndRotation(glm::vec3& pos, glm::quat& rot);

		// helper methods
		virtual physx::PxTransform getGlobalPose();
		glm::vec3 getRotationFromPxMat33(const physx::PxMat33& rotationMatrix);
		glm::vec3 getRotationFromPxTransform(const physx::PxTransform& transform);
	};

} // end namespace
