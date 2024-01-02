#include "KLS_PhysX_World.h"
#include "KLS_Logger.h"

#include "_KLS_PhysX.h"
#include "KLS_PhysX_Object.h"
#include "KLS_PhysX_Object_Static.h"
#include "KLS_PhysX_Object_Dynamic.h"
#include "KLS_PhysX_Object_Tree.h"
#include "KLS_PhysX_Object_Character.h"
#include "KLS_Mesh.h"
#include "KLS_MeshBuffer.h"
#include "KLS_Buffer.h"

namespace KLS
{
	KLS_PhysX_Object* KLS_PhysX_World::createPhysXObject(KLS_PhysX_ObjectCreationParams& params)
	{
		switch (params.Type)
		{
		case POT_CUBE: return createCube(params);			break;
		case POT_SPHERE: return createSphere(params);		break;
		case POT_TREE: return createTreeObject(params);		break;
		}

		return nullptr;
	}


	// setup some default values for our objects (stops us from having to type this info repeatedly)
	void KLS_PhysX_World::setupDefaultParams(physx::PxActor* actor, const KLS_PhysX_ObjectCreationParams& params)
	{
		if (!actor)
		{
			//printf("WARNING!!  setupDefaultParams() called on invalid actor\n");
			return;
		}

		// if this is a static obejct dont worry about mass and volocity type things
		if (actor->getType() == physx::PxActorType::eRIGID_STATIC)
		{
			// convert the actor into a valid static actor
			physx::PxRigidStatic* a2 = actor->is<physx::PxRigidStatic>();
			if (a2)
			{
				// if we are having debug info then set the apporpriate flag
				if (getPhysX()->getUseDebugInfo())
					a2->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);	// if debug mode then add visualizations for renderer

				a2->userData = reinterpret_cast<void*>(static_cast<uintptr_t>(params.ObjectId));

				// run through the shapes and set them up
				physx::PxShape* shapes[10];
				int numShapes = a2->getNbShapes();
				physx::PxU32 nShapes = a2->getShapes(shapes, numShapes > 9 ? 9 : numShapes);
				while (nShapes--)
				{
					shapes[nShapes]->userData = reinterpret_cast<void*>(static_cast<uintptr_t>(params.ObjectId));

					physx::PxFilterData params2 = shapes[nShapes]->getSimulationFilterData();
					params2.word0 = params.ObjectType;
					params2.word1 = params.ObjectType;
					shapes[nShapes]->setSimulationFilterData(params2);
					if (params.IsTrigger)
					{
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
					}
					else
					{
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
						shapes[nShapes]->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
					}
				}
			}
		}
		else
			if (actor->getType() == physx::PxActorType::eRIGID_DYNAMIC)
			{
				// convert the actor into a valid dynamic actor (we care about velocity and amss now :) )
				physx::PxRigidDynamic* a2 = actor->is<physx::PxRigidDynamic>();
				if (a2)
				{
					// if we are saving debug info then set the apporpriate flag
					if (getPhysX()->getUseDebugInfo())
						a2->setActorFlag(physx::PxActorFlag::eVISUALIZATION, true);	// if debug then enable visualizations

					a2->userData = reinterpret_cast<void*>(static_cast<uintptr_t>(params.ObjectId));

					a2->setAngularDamping(params.LinearDamping);									// slow the object rotation down each frame by this much
					a2->setLinearDamping(params.AngularDamping);										// slow the object velocity down each frame by this much
					a2->setLinearVelocity(physx::PxVec3(0, 0, 0));							// set the starting velocity to none

					// since we set the mass we need to update this
					physx::PxRigidBodyExt::updateMassAndInertia(*a2, physx::PxReal(params.Mass));

					physx::PxShape* shapes[10];
					int numShapes = a2->getNbShapes();
					physx::PxU32 nShapes = a2->getShapes(shapes, numShapes > 9 ? 9 : numShapes);
					while (nShapes--)
					{
						shapes[nShapes]->userData = reinterpret_cast<void*>(static_cast<uintptr_t>(params.ObjectId));

						shapes[nShapes]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
						physx::PxFilterData params2 = shapes[nShapes]->getSimulationFilterData();
						params2.word0 = params.ObjectType;
						params2.word1 = params.ObjectType;
						shapes[nShapes]->setSimulationFilterData(params2);
						if (params.IsTrigger)
						{
							shapes[nShapes]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, false);
							shapes[nShapes]->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, true);
						}
						else
						{
							shapes[nShapes]->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, true);
							shapes[nShapes]->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, false);
						}
					}
				}
			}
	}

	KLS_PhysX_Object* KLS_PhysX_World::createCube(KLS_PhysX_ObjectCreationParams& params)
	{
		glm::vec3 scale = params.Scale / 2.0f;
		float mass = params.Mass;
		if (mass <= 0) mass = 1;

		if (params.Dynamic)
		{
			physx::PxRigidDynamic* box = PxCreateDynamic(*getPhysics(), physx::PxTransform(physx::PxVec3(params.Position.x, params.Position.y, params.Position.z)), physx::PxBoxGeometry(scale.x, scale.y, scale.z), *getPhysX()->getDefaultMaterial(params.Material), mass);
			if (box)
			{
				setupDefaultParams(box, params);
				getScene()->addActor(*box);

				KLS_PhysX_Object_Dynamic* obj = new KLS_PhysX_Object_Dynamic();
					obj->initialize();
					obj->create(this, params);
					obj->setActor(box);
					obj->setPosition(params.Position);
					obj->setRotation(params.Rotation);
					obj->setIsTrigger(params.IsTrigger);
				return obj;
			}
		}
		else
		{
			physx::PxRigidStatic* box = PxCreateStatic(*getPhysics(), physx::PxTransform(physx::PxVec3(params.Position.x, params.Position.y, params.Position.z)), physx::PxBoxGeometry(scale.x, scale.y, scale.z), *getPhysX()->getDefaultMaterial(params.Material));
			if (box)
			{
				setupDefaultParams(box, params);
				getScene()->addActor(*box);

				KLS_PhysX_Object_Static* obj = new KLS_PhysX_Object_Static();
					obj->initialize();
					obj->create(this, params);
					obj->setActor(box);
					obj->setRotation(params.Rotation);
					obj->setIsTrigger(params.IsTrigger);
				return obj;
			}
		}

		return nullptr;
	}

	KLS_PhysX_Object* KLS_PhysX_World::createSphere(KLS_PhysX_ObjectCreationParams& params)
	{
		glm::vec3 scale = params.Scale / 2.0f;
		float mass = params.Mass;
		if (mass <= 0) mass = 1;

		if (params.Dynamic)
		{
			physx::PxRigidDynamic* sphere = PxCreateDynamic(*getPhysics(), physx::PxTransform(physx::PxVec3(params.Position.x, params.Position.y, params.Position.z)), physx::PxSphereGeometry(scale.x), *getPhysX()->getDefaultMaterial(params.Material), mass);
			if (sphere)
			{
				setupDefaultParams(sphere, params);
				getScene()->addActor(*sphere);

				KLS_PhysX_Object_Dynamic* obj = new KLS_PhysX_Object_Dynamic();
					obj->initialize();
					obj->create(this, params);
					obj->setActor(sphere);
					obj->setPosition(params.Position);
					obj->setRotation(params.Rotation);
					obj->setIsTrigger(params.IsTrigger);
				return obj;
			}
		}
		else
		{
			physx::PxRigidStatic* sphere = PxCreateStatic(*getPhysics(), physx::PxTransform(physx::PxVec3(params.Position.x, params.Position.y, params.Position.z)), physx::PxSphereGeometry(scale.x), *getPhysX()->getDefaultMaterial(params.Material));
			if (sphere)
			{
				setupDefaultParams(sphere, params);
				getScene()->addActor(*sphere);

				KLS_PhysX_Object_Static* obj = new KLS_PhysX_Object_Static();
					obj->initialize();
					obj->create(this, params);
					obj->setActor(sphere);
					obj->setRotation(params.Rotation);
					obj->setIsTrigger(params.IsTrigger);
				return obj;
			}
		}

		return nullptr;
	}

	KLS_PhysX_Object* KLS_PhysX_World::createTreeObject(const KLS_PhysX_ObjectCreationParams& data)
	{
		if (!data.Mesh) return nullptr;

		if (data.Dynamic) return createDynamicTreeObject(data);
		else return createStaticTreeObject(data);
	}

	KLS_PhysX_Object* KLS_PhysX_World::createStaticTreeObject(const KLS_PhysX_ObjectCreationParams& data)
	{
		KLS_PhysX_Object_Tree* obj = new KLS_PhysX_Object_Tree();
		obj->initialize();
		obj->create(this, data);

		for (uint32_t x = 0; x < data.Mesh->getMeshBufferCount(); x++)
				obj->addActor(createStaticTreeChunk(data, data.Mesh->getMeshBuffer(x)));

		obj->setRotation(data.Rotation);
		//		obj->setObjectType(data.objecttype);
		obj->setIsTrigger(data.IsTrigger);
		return obj;
	}

	physx::PxRigidStatic* KLS_PhysX_World::createStaticTreeChunk(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb)
	{
		KLS_TRACE(" createStaticTreeChunk -----------------------------------------------------------------------------------");

		physx::PxTriangleMesh* triMesh = triMeshFromMeshBuffer(data, mb);
		KLS_CHECK(triMesh, "triMeshFromMeshBuffer failed", nullptr);

		physx::PxRigidStatic* actor = getPhysX()->getPhysics()->createRigidStatic(physx::PxTransform(physx::PxVec3(data.Position.x, data.Position.y, data.Position.z)));
		if (!actor) { KLS_WARN("actor is not valid in addtreechunk() call"); return nullptr; }

		physx::PxShape* shape = getPhysics()->createShape(physx::PxTriangleMeshGeometry(triMesh), *getPhysX()->getDefaultMaterial(data.Material), true);
		if (!shape) { KLS_WARN("shape is not valid in addtreechunk() call"); return nullptr; }

		actor->attachShape(*shape);

		setupDefaultParams(actor, data);
		getScene()->addActor(*actor);

		return actor;
	}

	KLS_PhysX_Object* KLS_PhysX_World::createDynamicTreeObject(const KLS_PhysX_ObjectCreationParams& data)
	{
		KLS_PhysX_Object_TreeDynamic* obj = new KLS_PhysX_Object_TreeDynamic();
		obj->initialize();
		obj->create(this, data);

		for (uint32_t x = 0; x < data.Mesh->getMeshBufferCount(); x++)
				obj->addActor(createDynamicTreeChunk(data, data.Mesh->getMeshBuffer(x)));

		obj->setRotation(data.Rotation);
		//		obj->setObjectType(data.objecttype);
		obj->setIsTrigger(data.IsTrigger);
		return obj;
	}

	physx::PxRigidDynamic* KLS_PhysX_World::createDynamicTreeChunk(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb)
	{
		KLS_TRACE(" createStaticTreeChunk -----------------------------------------------------------------------------------");

		physx::PxTriangleMesh* triMesh = triMeshFromMeshBuffer(data, mb);
		KLS_CHECK(triMesh, "triMeshFromMeshBuffer failed", nullptr);

		physx::PxRigidDynamic* actor = getPhysX()->getPhysics()->createRigidDynamic(physx::PxTransform(physx::PxVec3(data.Position.x, data.Position.y, data.Position.z)));
		if (!actor) { KLS_WARN("actor is not valid in addtreechunk() call"); return nullptr; }

		physx::PxShape* shape = getPhysics()->createShape(physx::PxTriangleMeshGeometry(triMesh), *getPhysX()->getDefaultMaterial(data.Material), true);
		if (!shape) { KLS_WARN("shape is not valid in addtreechunk() call"); return nullptr; }

		actor->attachShape(*shape);

		setupDefaultParams(actor, data);
		getScene()->addActor(*actor);

		return actor;
	}

	physx::PxTriangleMesh* KLS_PhysX_World::triMeshFromMeshBuffer(const KLS_PhysX_ObjectCreationParams& data, KLS_MeshBuffer* mb)
	{
		KLS_CHECK(mb, "KLS_PhysXWorld::createStaticTreeChunk() meshbuffer is not valid", nullptr);

		KLS_VertexArray* VAO = mb->getVAO();
		KLS_CHECK(VAO, "KLS_PhysXWorld::createStaticTreeChunk() VAO is not valid", nullptr);

		KLS_VertexBuffer* VBO = VAO->getVertexBuffer(0);
		KLS_CHECK(VBO, "KLS_PhysXWorld::createStaticTreeChunk() VBO is not valid", nullptr);

		KLS_IndexBuffer* IBO = VAO->getIndexBuffer();
		KLS_CHECK(IBO, "KLS_PhysXWorld::createStaticTreeChunk() IBO is not valid", nullptr);

		std::vector<uint32_t> indexData = IBO->getData();
		int indexCount = (int)indexData.size();

		std::vector<float> vertexData = VBO->getData();
		int vertexStride = 8;
		int vertexCount = (int)vertexData.size() / vertexStride;

		std::vector<physx::PxVec3> verts;
		for (size_t i = 0; i < vertexData.size(); i += 8)
		{
			// Assuming data[i], data[i+1], and data[i+2] represent x, y, and z coordinates, respectively.
			physx::PxVec3 v = physx::PxVec3(vertexData[i], vertexData[i + 1], vertexData[i + 2]);
			v.x *= data.Scale.x;
			v.y *= data.Scale.y;
			v.z *= data.Scale.z;
			verts.push_back(v);
		}

		physx::PxTriangleMeshDesc meshDesc;
		meshDesc.points.count = (int)verts.size();
		meshDesc.points.data = &verts[0];
		meshDesc.points.stride = sizeof(physx::PxVec3);
		meshDesc.triangles.count = indexCount / 3;
		meshDesc.triangles.data = &indexData[0];
		meshDesc.triangles.stride = 3 * sizeof(physx::PxU32);

		physx::PxTolerancesScale cscale;
		physx::PxCookingParams params(cscale);

		// Create BVH33 midphase
		params.midphaseDesc = physx::PxMeshMidPhase::eBVH33;

		// we suppress the triangle mesh remap table computation to gain some speed, as we will not need it 
		params.suppressTriangleMeshRemapTable = true;

		// If DISABLE_CLEAN_MESH is set, the mesh is not cleaned during the cooking. The input mesh must be valid. 
		// The following conditions are true for a valid triangle mesh :
		//  1. There are no duplicate vertices(within specified vertexWeldTolerance.See PxCookingParams::meshWeldTolerance)
		//  2. There are no large triangles(within specified PxTolerancesScale.)
		// It is recommended to run a separate validation check in debug/checked builds, see below.
		params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_CLEAN_MESH);

		// If eDISABLE_ACTIVE_EDGES_PRECOMPUTE is set, the cooking does not compute the active (convex) edges, and instead 
		// marks all edges as active. This makes cooking faster but can slow down contact generation. This flag may change 
		// the collision behavior, as all edges of the triangle mesh will now be considered active.
		params.meshPreprocessParams &= ~static_cast<physx::PxMeshPreprocessingFlags>(physx::PxMeshPreprocessingFlag::eDISABLE_ACTIVE_EDGES_PRECOMPUTE);

		// The COOKING_PERFORMANCE flag for BVH33 midphase enables a fast cooking path at the expense of somewhat lower quality BVH construction.	
		params.midphaseDesc.mBVH33Desc.meshCookingHint = physx::PxMeshCookingHint::eSIM_PERFORMANCE;

		// using the default value
		params.midphaseDesc.mBVH33Desc.meshSizePerformanceTradeOff = 0.55f;

		PX_ASSERT(PxValidateTriangleMesh(params, meshDesc));

		physx::PxTriangleMesh* triMesh = NULL;
		physx::PxU32 meshSize = 0;

		// The cooked mesh may either be saved to a stream for later loading, or inserted directly into PxPhysics.
		if (1)
		{
			triMesh = PxCreateTriangleMesh(params, meshDesc, getPhysX()->getPhysics()->getPhysicsInsertionCallback());
		}
		else
		{
			physx::PxDefaultMemoryOutputStream outBuffer;
			PxCookTriangleMesh(params, meshDesc, outBuffer);
			physx::PxDefaultMemoryInputData stream(outBuffer.getData(), outBuffer.getSize());
			triMesh = getPhysX()->getPhysics()->createTriangleMesh(stream);
			meshSize = outBuffer.getSize();
		}

		// check if the mesh is valid, return nullptr if not
		if (!triMesh) { KLS_ERROR("WARNING traingle mesh is not valid"); return nullptr; }

		return triMesh;
	}

} // end namespace
