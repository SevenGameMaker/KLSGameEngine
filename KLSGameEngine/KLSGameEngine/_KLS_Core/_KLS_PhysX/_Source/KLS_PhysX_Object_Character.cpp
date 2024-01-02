// include this file only once
#pragma once

// include the needed header files
#include "KLS_PhysX_Object_Character.h"
#include "_KLS_PhysX.h"
#include "KLS_Logger.h"
#include "KLS_PhysX_World.h"
#include "KLS_Level.h"

// unclutter the golbal namespace
namespace KLS
{
	// do nothing constuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Character::KLS_PhysX_Object_Character() {}

	// do nothing destuctor - use initialize, create and cleanup instead
	KLS_PhysX_Object_Character::~KLS_PhysX_Object_Character() {}

	void KLS_PhysX_Object_Character::initialize()
	{
		KLS_PhysX_Object::initialize();
		m_ControllerState.collisionFlags = physx::PxControllerCollisionFlag::eCOLLISION_DOWN;
	}

	bool KLS_PhysX_Object_Character::create(KLS_PhysX_World* world, const KLS_PhysX_ObjectCreationParams& data)
	{
		if (!KLS_PhysX_Object::create(world, data)) return false;

		physx::PxF32	gInitialRadius = data.Scale.x;
		physx::PxF32	gInitialHeight = data.Scale.y;

//		if (data.Node)
//		{
//			gInitialRadius = data.node->getBoundingBox().getExtent().X / 2 * data.scale.X;
//			gInitialHeight = data.node->getBoundingBox().getExtent().Y / 3 * data.scale.Y * 2;
//		}

		physx::PxCapsuleControllerDesc desc;
		desc.position.x = data.Position.x;
		desc.position.y = data.Position.y;
		desc.position.z = data.Position.z;
		desc.radius = gInitialRadius;
		desc.height = gInitialHeight;
		desc.upDirection = physx::PxVec3(0, 1, 0);
		desc.slopeLimit = cosf(glm::radians(45.0f));
		desc.stepOffset = 0.5f;
//		desc.userData = (void*)data.userdata;
		desc.scaleCoeff = 0.9f;
		desc.volumeGrowth = 1.2f;
		desc.density = 10;
		desc.material = world->getPhysX()->getDefaultMaterial(0);
		desc.reportCallback = getWorld();
		//desc.behaviorCallback = getWorld();
		desc.climbingMode = physx::PxCapsuleClimbingMode::eCONSTRAINED;

		m_Controller = world->getControllerManager()->createController(desc);
		if (m_Controller)
		{
			m_Controller->setUserData((void*)data.Type);
			m_Controller->getActor()->userData = (void*)data.Type;
		}
		else { KLS_WARN("create() Controller creaation failed"); return false; }

		// everything went fine
		return true;
	}

	bool KLS_PhysX_Object_Character::cleanup()
	{
		// safely release the controller
		if (m_Controller) { m_Controller->release(); m_Controller = 0; }

		// always return false form a cleanup function
		return KLS_PhysX_Object::cleanup();
	}

	physx::PxTransform KLS_PhysX_Object_Character::getGlobalPose()
	{
		return m_Controller->getActor()->getGlobalPose();
	}

	glm::vec3 KLS_PhysX_Object_Character::getPosition()
	{
		glm::vec3 pos(0, 0, 0);
		if (m_Controller)
		{
			physx::PxExtendedVec3 p = m_Controller->getPosition();
			pos = glm::vec3((float)p.x, (float)p.y, (float)p.z);
		}
		return pos;
	}

	void KLS_PhysX_Object_Character::setPosition(glm::vec3 pos)
	{
		physx::PxExtendedVec3 p(pos.x, pos.y, pos.z);
		if (m_Controller) m_Controller->setPosition(p);
	}

	void KLS_PhysX_Object_Character::drawDebugInfo(glm::vec4 color)
	{
		if (!m_Controller) return;
		if (!m_Controller->getActor()) return;

#if(0)
		// get a pointer to the device
		IrrlichtDevice* device = getWorld()->getLevel()->getDevice();
		if (!device) return;

		core::matrix4 mat;
		video::SMaterial material;
		material.TextureLayer->Texture = 0;
		material.Lighting = false;
		device->getVideoDriver()->setMaterial(material);
		device->getVideoDriver()->setTransform(video::ETS_WORLD, core::matrix4());
		{
			physx::PxBounds3 dest = m_Controller->getActor()->getWorldBounds();
			const core::aabbox3d<f32> box(dest.minimum.x, dest.minimum.y, dest.minimum.z, dest.maximum.x, dest.maximum.y, dest.maximum.z);
			device->getVideoDriver()->draw3DBox(box, color);
		}
#endif
	}

	void KLS_PhysX_Object_Character::setFreeze(bool value)
	{
		if (!m_Controller) return;
		if (!m_Controller->getActor()) return;
		if (!value) m_Controller->getActor()->wakeUp();
		else m_Controller->getActor()->putToSleep();
	}
	void KLS_PhysX_Object_Character::setMass(float value)
	{
		if (!m_Controller) return;
		if (!m_Controller->getActor()) return;
		m_Controller->getActor()->setMass(value);
	}
	float KLS_PhysX_Object_Character::getMass()
	{
		if (!m_Controller) return 1;
		if (!m_Controller->getActor()) return 1;
		return m_Controller->getActor()->getMass();
	}
	void KLS_PhysX_Object_Character::setLinearDamping(float value)
	{
		if (!m_Controller) return;
		if (!m_Controller->getActor()) return;
		m_Controller->getActor()->setLinearDamping(value);
	}
	void KLS_PhysX_Object_Character::setAngularDamping(float value)
	{
		if (!m_Controller) return;
		if (!m_Controller->getActor()) return;
		m_Controller->getActor()->setAngularDamping(value);
	}
	void KLS_PhysX_Object_Character::addForce(glm::vec3 dir, float force)
	{
		if (!m_Controller) { KLS_ERROR("no controller?"); return; }
		if (!m_Controller->getActor()) { KLS_ERROR("no controller actor?"); return; }
		if (force <= 0) { return; }

		//dir = glm::normalize(dir);
		physx::PxVec3 disp = physx::PxVec3(dir.x, dir.y, dir.z) * force;
		m_TotalForce += glm::vec3(disp.x, disp.y, disp.z);
	}

	void KLS_PhysX_Object_Character::preFrame(const float & elapsedtime)
	{
		float et = elapsedtime;

		// add the gravity each frame
		glm::vec3 tf = getTotalForce();
		m_TotalForce = glm::vec3(0, 0, 0);
		physx::PxVec3 disp(tf.x, tf.y, tf.z);
		moveCharacter(disp, et, 0);
	}

	void KLS_PhysX_Object_Character::moveCharacter(const physx::PxVec3& dispVector, physx::PxF32 elapsedTime, physx::PxU32 collisionGroups)
	{
		if (!m_Controller) { KLS_ERROR("no controller?"); return; }
		if (!m_Controller->getActor()) { KLS_ERROR("no controller actor?"); return; }

		// adjust to movement over time
		physx::PxVec3 d = dispVector * elapsedTime;

		// make the move (which updates the states inside m_Controller->getState(state);
		m_Controller->move(d, 0.0001f, elapsedTime, nullptr);

		// remember the state
		m_Controller->getState(m_ControllerState);
	}

//	bool KLS_PhysX_Object_Character::filter(const physx::PxController& a, const physx::PxController& b)
//	{
//		return true;
//	}

	bool KLS_PhysX_Object_Character::isOnGround()
	{
		return (bool)(m_ControllerState.collisionFlags & physx::PxControllerCollisionFlag::eCOLLISION_DOWN);
	}

	void KLS_PhysX_Object_Character::crouch(bool v, float smallsize, float large)
	{
		if (m_IsCrouching == v) return;

		if (m_IsCrouching)
		{
			// stand up
			m_Controller->resize(smallsize);
		}
		else
		{
			// crouch down
			m_Controller->resize(large);
		}
		m_IsCrouching = v;
	}

	// attach this to an object
	void KLS_PhysX_Object_Character::setUserData(int id)
	{
		if (m_Controller)
		{
			m_Controller->getActor()->userData = (void*)(intptr_t)id;
			physx::PxShape* shapes[10];
			physx::PxU32 nShapes = m_Controller->getActor()->getShapes(shapes, 10);
			while (nShapes--)
			{
				shapes[nShapes]->userData = (void*)(intptr_t)id;
			}
		}
	}

	void KLS_PhysX_Object_Character::setObjectType(KLS_OBJECT_TYPE t, KLS_OBJECT_TYPE collideswith)
	{
		if (m_Controller)
		{
			physx::PxShape* shapes[10];
			physx::PxU32 nShapes = m_Controller->getActor()->getShapes(shapes, 10);
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

} // end namespace KLS
