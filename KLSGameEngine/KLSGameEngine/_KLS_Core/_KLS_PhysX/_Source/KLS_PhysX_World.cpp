#include "KLS_PhysX_World.h"
#include "KLS_Logger.h"
#include "_KLS_PhysX.h"
#include "KLS_Level.h"
#include "KLS_Application.h"

namespace KLS
{
	physx::PxFoundation* KLS_PhysX_World::getFoundation() { return m_PhysX ? m_PhysX->getFoundation() : nullptr; }
	physx::PxPhysics* KLS_PhysX_World::getPhysics() { return m_PhysX ? m_PhysX->getPhysics() : nullptr; }

	KLS_PhysX_World::KLS_PhysX_World()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_World::KLS_PhysX_World()");
	}

	KLS_PhysX_World::~KLS_PhysX_World()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_World::~KLS_PhysX_World()");
	}

	void KLS_PhysX_World::initialize()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_World::initialize()");
	}

	bool KLS_PhysX_World::create(KLS_Level* level)
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_World::create()");

		m_Level = level;
		m_PhysX = level->getApplication()->getPhysX();

		//Create the scene
		physx::PxSceneDesc sceneDesc(getPhysX()->getPhysics()->getTolerancesScale());	//Descriptor class for scenes 
		sceneDesc.sanityBounds.minimum.x = -PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.sanityBounds.minimum.y = -PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.sanityBounds.minimum.z = -PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.sanityBounds.maximum.x = PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.sanityBounds.maximum.y = PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.sanityBounds.maximum.z = PX_MAX_BOUNDS_EXTENTS;
		sceneDesc.gravity = physx::PxVec3(0.0f, KLS_PHYSX_GRAVITY, 0.0f);
		sceneDesc.cpuDispatcher = m_PhysX->getCpuDispatcher();
		sceneDesc.filterShader = FilterShader;
		sceneDesc.simulationEventCallback = this;

		if (getPhysX()->getCudaContextManager())
		{
			KLS_TRACE(" KLS_PhysXWorld::create() GPU enabled PhysX is valid");
			sceneDesc.flags |= physx::PxSceneFlag::eENABLE_GPU_DYNAMICS;
			sceneDesc.broadPhaseType = physx::PxBroadPhaseType::eGPU;
			sceneDesc.cudaContextManager = getPhysX()->getCudaContextManager();
			sceneDesc.sceneQueryUpdateMode = physx::PxSceneQueryUpdateMode::eBUILD_ENABLED_COMMIT_DISABLED;
			sceneDesc.gpuMaxNumPartitions = 8;
			sceneDesc.gpuDynamicsConfig.tempBufferCapacity *= 2;
		}
		else KLS_WARN(" KLS_PhysXWorld::create() GPU enabled PhysX is not valid");

		//Creates a scene 
		m_Scene = getPhysX()->getPhysics()->createScene(sceneDesc);
		if (!m_Scene)
		{
			KLS_ERROR(" KLS_PhysXWorld::create() m_Scene not valid");
			return false;
		}

		if (m_PhysX->getUseDebugInfo())
		{
			KLS_WARN(" KLS_PhysXWorld::create() visualizing physx info");

			// set any flags that you desire but might need to set flags on actors and filters also
			m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eSCALE, 1);
			m_Scene->setVisualizationParameter(physx::PxVisualizationParameter::eCOLLISION_SHAPES, 1);
		}

		// create the character controller manager
		m_ControllerManager = PxCreateControllerManager(*m_Scene);
 
		if (m_PhysX->getPvd())
		{
			// if in debug mode, setup the pvd (slows down simulation so dont use in release modes)
			physx::PxPvdSceneClient* pvdClient = m_Scene->getScenePvdClient();
			if (pvdClient)
			{
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONSTRAINTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_CONTACTS, true);
				pvdClient->setScenePvdFlag(physx::PxPvdSceneFlag::eTRANSMIT_SCENEQUERIES, true);
			}
		}
		// everything went fine
		return KLS_SUCCESS;
	}

	bool KLS_PhysX_World::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_PhysX_World::KLS_PhysX_World()");

		KLS_SAFE_RELEASE(m_ControllerManager);
		KLS_SAFE_RELEASE(m_Scene);
		m_PhysX = nullptr;
		m_Level = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_PhysX_World::preFrame(const double& elapsedtime)
	{
		if (m_Scene) m_Scene->simulate((physx::PxReal)elapsedtime);
		m_Scene->fetchResults(true);
	}

}