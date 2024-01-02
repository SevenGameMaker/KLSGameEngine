// include the needed header files
#include "myLevel_002.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_ImGui.h"
#include "KLS_Camera_FPS.h"
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include "_KLS_ECS_Systems.h"
#include "KLS_Mesh.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	myLevel_002::myLevel_002() : KLS_Level()
	{
		// log this activity
		KLS_TRACE("myLevel_002::myLevel_002()");
	}

	myLevel_002::~myLevel_002()
	{
		// log this activity
		KLS_TRACE("myLevel_002::~myLevel_002()");
	}

	// dual creation allows for better error handling
	bool myLevel_002::create(KLS_Application* app, int id)
	{
		// log this activity
		KLS_TRACE("myLevel_002::create()");

		// call the base class
		KLS_CHECK(KLS_Level::create(app, id), "base class creation failed", KLS_FAILURE);

		m_Camera = new KLS_Camera_FPS(getDriver(), getKeyMap_WASD());

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool myLevel_002::cleanup()
	{
		// log this activity
		KLS_TRACE("myLevel_002::cleanup()");

		if (m_Camera) delete(m_Camera); m_Camera = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_Level::cleanup();
	}

	void myLevel_002::load()
	{
		// log this activity
		KLS_TRACE("myLevel_002::load()");

		// set the active camera
		if (m_Camera) getDriver()->setActiveCamera(m_Camera);
		getDriver()->setAmbientIntensity(0.5f);

		createWorld();

		// call the base class
		KLS_Level::load();
	}

	void myLevel_002::unload()
	{
		// log this activity
		KLS_TRACE("myLevel_002::unload()");

		destroyWorld();

		// call the base class
		KLS_Level::unload();
	}

	void myLevel_002::createWorld()
	{
		if (!getDriver()->getResourceManager()->getCubemap("skybox2"))
		{
			std::string path = "../../../../_media/_Assets/_Textures/_skybox/space/green/";
			getDriver()->getResourceManager()->createCubemap("skybox2", false, path + "right.png", path + "left.png", path + "top.png", path + "bottom.png", path + "front.png", path + "back.png");
		}

		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/skybox.obj", KLSVT3D);
			Mesh->getMaterial(0).Cubemap[0] = "skybox2";
			Mesh->getMaterial(0).MaterialType = KLSMT_SKYBOX;
		KLS_Entity e = getECS()->createEntity(getUniqueId(), "Skybox", KLS_Transform());
			e.addComponent<KLS_COMPONENT_SKYBOX>(Mesh);

		glm::vec3 pos(-20, 0, 0);
		for (int z = 0; z < 4; z++)
		{
			int count = 4;
			float offset = 2.0f;
			glm::vec3 scale(10);
			glm::vec3 currentscale(10);
			glm::vec3 currentpos(0, 0, z * scale.z + (offset * z));
			currentpos += pos;
			KLS_Entity last;

			for (int x = 0; x < count; x++)
			{
				KLS_Entity e = createCube(last,getUniqueId(),KLS_Transform(currentpos, KLS_IDENTITY_QUAT, currentscale), false, false, glm::vec3(0), 0);
				last = e;
				currentscale = glm::vec3(1);
				currentpos = glm::vec3(0, scale.y + offset, 0);
			}
		}

		createPyramidOfCubes(glm::vec3(0), 7, 7, 2,false,false);
	}

	void myLevel_002::destroyWorld()
	{
		getECS()->clear();
	}

	bool myLevel_002::onRMousePressed(const KLS_Event& e)
	{
		if (m_Camera) m_Camera->setActive(!m_Camera->getActive());
		// let the event die
		return KLS_SUCCESS;
	}

	bool myLevel_002::onLMousePressed(const KLS_Event& e)
	{
		KLS_Entity entity = getSelectedObject(e.Mouse.X, e.Mouse.Y);
		getECS()->removeEntity(entity);

		// let the event die
		return KLS_SUCCESS;
	}

	// imgui related methods
	void myLevel_002::ImGui_draw()
	{
		// call the base class
		KLS_Level::ImGui_draw();
	}

} // end namespace