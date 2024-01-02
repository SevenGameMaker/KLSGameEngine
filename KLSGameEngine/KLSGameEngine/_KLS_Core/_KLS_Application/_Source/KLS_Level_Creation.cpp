#include "KLS_Level.h"
#include "KLS_Entity.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_PhysX_World.h"
#include "KLS_PhysX_Object.h"
#include "_KLS_ECS_Components.h"

namespace KLS
{
	void KLS_Level::rescalePhysxObject(KLS_Entity e)
	{
		// if the entity has a physx component then adjust it
		if (e.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
		{
			auto& po = e.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
			if (po.m_PhysxObject)
			{
				// we have to recreate the po
				KLS_PhysX_ObjectCreationParams params = po.m_PhysxObject->getParams();

				// remove the old po
				po.m_PhysxObject->cleanup();
				delete(po.m_PhysxObject);

				// create a new physx object
				params.Scale = e.getTransform().getScale();
				po.m_PhysxObject = getPhysXWorld()->createPhysXObject(params);
			}
		}
	}

	KLS_Entity KLS_Level::createSkybox(std::string skyboxtexturename, std::string texturepath)
	{
		if (!getDriver()->getResourceManager()->getCubemap(skyboxtexturename))
		{
			std::string path = texturepath;
			getDriver()->getResourceManager()->createCubemap(skyboxtexturename, false, path + "right.png", path + "left.png", path + "top.png", path + "bottom.png", path + "front.png", path + "back.png");
		}

		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/skybox.obj", KLSVT3D);
		if (Mesh)
		{
			Mesh->getMaterial(0).Cubemap[0] = skyboxtexturename;
			Mesh->getMaterial(0).MaterialType = KLSMT_SKYBOX;
		}
		KLS_Entity e = getECS()->createEntity(getUniqueId(), "Skybox", KLS_Transform());
			e.addComponent<KLS_COMPONENT_SKYBOX>(Mesh);
		return e;
	}

	KLS_Entity KLS_Level::createLight(KLS_Entity parent, KLS_UUID id, KLS_Transform t, glm::vec4 color)
	{
		KLS_Entity le = getECS()->createEntity(id, "Light", t);
		KLS_COMPONENT_LIGHT ld;
		le.addComponent<KLS_COMPONENT_LIGHT>(ld);
		KLS_Mesh* lightMesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/light.obj", KLSVT3D);
		if (lightMesh)
		{
			lightMesh->getMaterial(0).ColorDiffuse = ld.m_LightData.color;
			lightMesh->getMaterial(0).MaterialType = KLSMT_COLOR;
		}
		le.addComponent<KLS_COMPONENT_MESH>(lightMesh);
		if (parent.getEntity() != entt::null) parent.addChild(le);
		return le;
	}

	KLS_Entity KLS_Level::createCube(KLS_Entity parent, KLS_UUID id, KLS_Transform t,
		bool physx, bool dynamic, glm::vec3 dir, float velocity)
	{
		glm::vec3 pos = t.getPosition();
		glm::vec3 scale = t.getScale();

		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/cube.obj", KLSVT3D);
		if (Mesh)
		{
			Mesh->getMaterial(0).Texture_Diffuse[0] = "../../../../_media/_assets/_textures/default.png";
			Mesh->getMaterial(0).MaterialType = KLSMT_TEXTURED;
		}
		KLS_Entity e = getECS()->createEntity(id, "cube", t);
		e.addComponent<KLS_COMPONENT_MESH>(Mesh);

		glm::vec3 physxpos = pos;
		glm::vec3 physxscale = scale;

		if (!parent.isNull())
		{
			auto& t = parent.getTransform();
			//physxpos += t.getPosition();
			physxscale = t.getScale();
			parent.addChild(e);
		}

		if (physx)
		{
			KLS_COMPONENT_PHYSXOBJECT po;
			KLS_PhysX_ObjectCreationParams params;
			params.Dynamic = dynamic;
			params.Mass = 100.0f;
			params.Position = physxpos;
			params.Scale = physxscale;
			params.Type = POT_CUBE;
			params.ObjectType = KLSOT_OBJECT;
			params.ObjectId = id;
			po.m_PhysxObject = getPhysXWorld()->createPhysXObject(params);
			po.m_PhysxObject->addForce(dir, velocity);
			if (po.m_PhysxObject) e.addComponent<KLS_COMPONENT_PHYSXOBJECT>(po);
		}

		return e;
	}

	void KLS_Level::createStackOfCubes(glm::vec3 pos, glm::vec3 scale, int count, bool physics, bool dynamic)
	{
		float offset = 0.1f;
		for (int i = 0; i < count; i++)
		{
			glm::vec3 position = pos+glm::vec3(0, (offset * i) + i * scale.y, 0);
			KLS_Entity cube = createCube(KLS_Entity(), getUniqueId(), KLS_Transform(position), physics, dynamic, glm::vec3(0), 0);
		}
	}

	void KLS_Level::createPyramidOfCubes(glm::vec3 pos, int baseXCount, int baseZCount, float cubeSize, bool physics, bool dynamic)
	{
		float halfWidth = baseXCount * cubeSize * 0.5f;
		float halfDepth = baseZCount * cubeSize * 0.5f;
		float yPos = 0.0f;
		float y = 0;
		float offset = 0.1f;
		while (baseXCount > 0 && baseZCount > 0)
		{
			for (auto z = y; z < baseZCount; ++z)
			{
				for (auto x = y; x < baseXCount; ++x)
				{
					float xPos = x * cubeSize - halfWidth + cubeSize * 0.5f;
					float zPos = z * cubeSize - halfDepth + cubeSize * 0.5f;
					glm::vec3 position((x * offset) + pos.x+xPos , (y * offset) + pos.y+yPos , (z * offset) + pos.z+zPos);
					createCube(KLS_Entity(), getUniqueId(), KLS_Transform(position, KLS_IDENTITY_QUAT, glm::vec3(cubeSize)), physics, dynamic, glm::vec3(0), 0);
				}
			}

			// Decrease the base dimensions for the next layer of cubes
			baseXCount--;
			baseZCount--;
			y++;

			// Increase the Y position for the next layer of cubes
			yPos += cubeSize;
		}
	}

	KLS_Entity KLS_Level::createMesh(KLS_Entity parent, KLS_UUID id, KLS_Transform t, std::string meshfilename, KLS_MATERIAL_TYPE mt, bool physx)
	{
		glm::vec3 pos = t.getPosition();
		glm::vec3 scale = t.getScale();

		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh(meshfilename, KLSVT3D);
		if (Mesh)
		{
			for (int x=0; x<Mesh->getMaterialCount(); x++)
				Mesh->getMaterial(x).MaterialType = mt;
		}
		KLS_Entity e = getECS()->createEntity(id, "mesh", t);
			e.addComponent<KLS_COMPONENT_MESH>(Mesh);

		glm::vec3 physxpos = pos;
		glm::vec3 physxscale = scale;

		if (!parent.isNull())
		{
			auto& t = parent.getTransform();
			//physxpos += t.getPosition();
			physxscale = t.getScale();
			parent.addChild(e);
		}

		if (physx)
		{
			KLS_COMPONENT_PHYSXOBJECT po;
			KLS_PhysX_ObjectCreationParams params;
			params.Dynamic = false;
			params.Mass = 100.0f;
			params.Position = physxpos;
			params.Scale = physxscale;
			params.Type = POT_TREE;
			params.ObjectType = KLSOT_OBJECT;
			params.ObjectId = id;
			params.Mesh = Mesh;
			po.m_PhysxObject = getPhysXWorld()->createPhysXObject(params);
			if (po.m_PhysxObject != nullptr)	e.addComponent<KLS_COMPONENT_PHYSXOBJECT>(po);
		}

		return e;
	}

	KLS_Entity KLS_Level::createGrid()
	{
		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/plane.obj", KLSVT3D);
		if (Mesh)
		{
			Mesh->getMaterial(0).ColorDiffuse = KLS_WHITE;
			Mesh->getMaterial(0).MaterialType = KLSMT_INFINITEPLANE;
			Mesh->getMaterial(0).Transparency = 1.0;
		}
		KLS_Entity e = getECS()->createEntity(getUniqueId(), "grid", KLS_Transform(glm::vec3(0), KLS_IDENTITY_QUAT, glm::vec3(10)));
			e.addComponent<KLS_COMPONENT_MESH>(Mesh);
		return e;
	}

} // end namespace