#pragma once
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include <PxPhysicsAPI.h>      // For including all core PhysX headers
#include <foundation/PxVec3.h> // For using PxVec3 (optional)
#include "KLS_PhysX_ObjectCreationParams.h"
#include "KLS_MeshBuffer.h"

namespace KLS
{
	typedef class KLS_PhysX KLS_PhysX;
	typedef class KLS_PhysX_Object KLS_PhysX_Object;
	typedef class KLS_Level KLS_Level;

	class KLS_PhysX_World :
		public physx::PxSimulationEventCallback,
		public physx::PxControllerFilterCallback,
		public physx::PxUserControllerHitReport
	{
	private:
		KLS_VARIABLE_GET(KLS_PhysX*, PhysX, nullptr);
		KLS_VARIABLE_GET(physx::PxScene*, Scene, nullptr);
		KLS_VARIABLE_GET(physx::PxControllerManager*, ControllerManager, nullptr);
		KLS_VARIABLE_GET(KLS_Level*, Level, nullptr);

	public:
		physx::PxFoundation* getFoundation();
		physx::PxPhysics* getPhysics();

	public:
		KLS_PhysX_World();
		virtual ~KLS_PhysX_World();

		virtual void initialize();
		virtual bool create(KLS_Level* level);
		virtual bool cleanup();

		virtual void preFrame(const double& elapsedtime);

		// setup some default values for our objects (stops us from having to type this info repeatedly)
		void setupDefaultParams(physx::PxActor* actor, const KLS_PhysX_ObjectCreationParams& data);

	public:
		// PhysX callback functions
		
		// PxSimulationEventCallback
		virtual void onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count);
		virtual void onWake(physx::PxActor** actors, physx::PxU32 count);
		virtual void onSleep(physx::PxActor** actors, physx::PxU32 count);
		virtual void onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs);
		virtual void onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count);
		virtual void onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count);

		// PxControllerFilterCallback
		virtual bool filter(const physx::PxController& a, const physx::PxController& b);

		// PxUserControllerHitReport
		virtual void onShapeHit(const physx::PxControllerShapeHit& hit);
		virtual void onControllerHit(const physx::PxControllersHit& hit);
		virtual void onObstacleHit(const physx::PxControllerObstacleHit& hit);

	
		// object creation methods
		virtual KLS_PhysX_Object* createPhysXObject(KLS_PhysX_ObjectCreationParams& params);
		virtual KLS_PhysX_Object* createCube(KLS_PhysX_ObjectCreationParams& params);
		virtual KLS_PhysX_Object* createSphere(KLS_PhysX_ObjectCreationParams& params);
		virtual KLS_PhysX_Object* createTreeObject(const KLS_PhysX_ObjectCreationParams& data);
		virtual KLS_PhysX_Object* createDynamicTreeObject(const KLS_PhysX_ObjectCreationParams& data);
		virtual KLS_PhysX_Object* createStaticTreeObject(const KLS_PhysX_ObjectCreationParams& data);

	private:
		physx::PxRigidStatic* createStaticTreeChunk(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb);
		physx::PxRigidDynamic* createDynamicTreeChunk(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb);
		physx::PxTriangleMesh* triMeshFromMeshBuffer(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb);
	};

	extern physx::PxFilterFlags FilterShader(
		physx::PxFilterObjectAttributes attributes0, physx::PxFilterData filterData0,
		physx::PxFilterObjectAttributes attributes1, physx::PxFilterData filterData1,
		physx::PxPairFlags& pairFlags, const void* constantBlock, physx::PxU32 constantBlockSize);
}