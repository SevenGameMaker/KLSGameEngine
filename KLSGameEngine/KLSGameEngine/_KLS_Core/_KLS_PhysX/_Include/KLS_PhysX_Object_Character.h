// include this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_PhysX_Object.h"
#include "KLS_PhysX_ObjectCreationParams.h"
#include "KLS_Event.h"

// unclutter the golbal namespace
namespace KLS
{
	class KLS_API KLS_PhysX_Object_Character : public KLS_PhysX_Object
	{
	public:

		// class variables
	protected:
		KLS_VARIABLE(physx::PxController*, Controller, nullptr);		// the physx controller

		KLS_VARIABLE_SETGET(	glm::vec3,	TotalForce,		glm::vec3(0));	// current amount of force on the object
		KLS_VARIABLE_GET(		glm::vec3,	LastTotalForce, glm::vec3(0));	// last known amount of force on the object
		KLS_VARIABLE_GET(		bool,		IsCrouching,	false);
		physx::PxControllerState m_ControllerState;

	public:
		// do nothing constuctor - use initialize, create and cleanup instead
		KLS_PhysX_Object_Character();

		// do nothing destuctor - use initialize, create and cleanup instead
		virtual ~KLS_PhysX_Object_Character();

		// set all variable to a knoen value
		void initialize();

		// dual creaion allows for better error handling
		bool create(KLS_PhysX_World* world, const KLS_PhysX_ObjectCreationParams& data);

		// cleanup whatever memory mess we made
		bool cleanup();

		// frame method
		virtual void preFrame(const float& elapsedtime);

		virtual void drawDebugInfo(glm::vec4 color);

		// override due to controller
		virtual physx::PxTransform	getGlobalPose();
		virtual glm::vec3			getPosition();
		virtual float				getMass();
		virtual void				setPosition(glm::vec3 pos);
		virtual void				setFreeze(bool value);
		virtual void				setMass(float value);
		virtual void				setLinearDamping(float value);
		virtual void				setAngularDamping(float value);

		// add a force to the object
		virtual void addForce(glm::vec3 dir, float magnitude);

		virtual void moveCharacter(const physx::PxVec3& dispVector, physx::PxF32 elapsedTime, physx::PxU32 collisionGroups);

//		virtual bool filter(const physx::PxController& a, const physx::PxController& b);

		virtual void setUserData(int id);												// set the userdata normally this is the id of the container object
		virtual void setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE colldieswith);	// used in collision filtering 

		bool isOnGround();
		void crouch(bool v, float smallsize, float large);

	};

} // end namespace KLS
