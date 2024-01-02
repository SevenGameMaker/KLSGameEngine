// include the needed header files
#include "KLS_PhysX_World.h"

#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "KLS_Level.h"
#include "_KLS_PhysX.h"
#include "KLS_PhysX_Object.h"
#include "KLS_Message.h"

// unclutter the global namespace
namespace KLS
{
	physx::PxFilterFlags FilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize)
	{
		// let triggers through
		if (physx::PxFilterObjectIsTrigger(attributes0) || physx::PxFilterObjectIsTrigger(attributes1))
		{
			pairFlags = physx::PxPairFlag::eTRIGGER_DEFAULT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			return physx::PxFilterFlag::eDEFAULT;
		}

		// Extract object type and allowed collision bitmask from filter data
		uint64_t objectType0 = filterData0.word0;
		uint64_t allowedCollisions0 = filterData0.word1;
		uint64_t objectType1 = filterData1.word0;
		uint64_t allowedCollisions1 = filterData1.word1;

		// Check if the collision is allowed based on object type and allowed collisions
		if ((objectType0 & allowedCollisions1) || (objectType1 & allowedCollisions0))
		{
			pairFlags = physx::PxPairFlag::eCONTACT_DEFAULT; // Allow collision
			pairFlags |= physx::PxPairFlag::eDETECT_DISCRETE_CONTACT;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_FOUND;
			pairFlags |= physx::PxPairFlag::eNOTIFY_TOUCH_LOST;
			return physx::PxFilterFlag::eDEFAULT;
		}
		else
		{
			KLS_ERROR("collision not allowed");
			pairFlags &= ~physx::PxPairFlag::eCONTACT_DEFAULT; // Disable collision
			return physx::PxFilterFlag::eKILL;
		}
	}

	////////////////////////////////////
	// PxControllerFilterCallback
	////////////////////////////////////

	// should two controllers collide?
	bool KLS_PhysX_World::filter(const physx::PxController& a, const physx::PxController& b)
	{
		KLS_WARN("KLS_PhysX_World::filter");

		// Cast the controllers to PxController
		const physx::PxController* controllerA = &a;
		const physx::PxController* controllerB = &b;

		// Retrieve the shapes from the controllers
		physx::PxShape* shapesA[10];
		physx::PxU32 numShapesA = controllerA->getActor()->getShapes(shapesA, 10);

		physx::PxShape* shapesB[10];
		physx::PxU32 numShapesB = controllerB->getActor()->getShapes(shapesB, 10);

		// Extract the collision filter data from the shapes
		physx::PxFilterData filterDataA = shapesA[0]->getSimulationFilterData();
		physx::PxFilterData filterDataB = shapesB[0]->getSimulationFilterData();

		KLS_OBJECT_TYPE objectTypeA = static_cast<KLS_OBJECT_TYPE>(filterDataA.word0);
		KLS_OBJECT_TYPE allowedCollisionsA = static_cast<KLS_OBJECT_TYPE>(filterDataA.word1);

		KLS_OBJECT_TYPE objectTypeB = static_cast<KLS_OBJECT_TYPE>(filterDataB.word0);
		KLS_OBJECT_TYPE allowedCollisionsB = static_cast<KLS_OBJECT_TYPE>(filterDataB.word1);

		if ((objectTypeA & allowedCollisionsB) || (objectTypeB & allowedCollisionsA))
		{
			return true; // Allow collision
			//return physx::PxFilterFlag::eNOTIFY; // Allow collision
		}
		else
		{
			return false; // Disable collision
			//return physx::PxFilterFlag::eKILL; // Disable collision
		}

//		return physx::PxFilterFlag::eDEFAULT;
		return false;
	}

	////////////////////////////////////
	// PxSimulationEventCallback
	////////////////////////////////////

	void KLS_PhysX_World::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		KLS_WARN("KLS_PhysX_World::onConstraintBreak");
	}

	void KLS_PhysX_World::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		KLS_WARN("KLS_PhysX_World::onWake");
	}

	void KLS_PhysX_World::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		KLS_WARN("KLS_PhysX_World::onSleep");
	}

	void KLS_PhysX_World::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		//KLS_WARN("KLS_PhysX_World::onContact");

		if (nbPairs > 0)
		{
			KLS_Message msg;
			msg.DeliveryTime = 0;
			msg.Dest = (int)reinterpret_cast<uintptr_t>(pairs->shapes[0]->userData);
			msg.Source = (int)reinterpret_cast<uintptr_t>(pairs->shapes[1]->userData);
			msg.Type = KLS_MT_COLLISION;
			m_Level->onMessage(msg);

			KLS_Message msg2;
			msg2.DeliveryTime = 0;
			msg2.Dest = (int)reinterpret_cast<uintptr_t>(pairs->shapes[1]->userData);
			msg2.Source = (int)reinterpret_cast<uintptr_t>(pairs->shapes[0]->userData);
			msg2.Type = KLS_MT_COLLISION;
			m_Level->onMessage(msg2);
		}
	}

	void KLS_PhysX_World::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		KLS_WARN("KLS_PhysX_World::onTrigger");
	}

	void KLS_PhysX_World::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		KLS_WARN("KLS_PhysX_World::onAdvance");
	}

	////////////////////////////////////
	// PxUserControllerHitReport
	////////////////////////////////////
	void KLS_PhysX_World::onShapeHit(const physx::PxControllerShapeHit& hit)
	{
		// this gets called too often since character is always on the ground
		// KLS_WARN("KLS_PhysX_World::onShapeHit");
	}

	void KLS_PhysX_World::onControllerHit(const physx::PxControllersHit& hit)
	{
		KLS_WARN("KLS_PhysX_World::onControllerHit");
	}

	void KLS_PhysX_World::onObstacleHit(const physx::PxControllerObstacleHit& hit)
	{
		KLS_WARN("KLS_PhysX_World::onObstacleHit");
	}

} // end namespace
