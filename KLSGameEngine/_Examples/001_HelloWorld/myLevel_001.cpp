// include the needed header files
#include "myLevel_001.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_Math.h"
#include "KLS_Camera_FPS.h"
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include "_KLS_ECS_Systems.h"
#include "KLS_Mesh.h"
#include "KLS_ImGui.h"
#include "KLS_SolarSystem.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	myLevel_001::myLevel_001() : KLS_Level()
	{
		// log this activity
		KLS_TRACE("myLevel_001::myLevel_001()");
	}

	myLevel_001::~myLevel_001()
	{
		// log this activity
		KLS_TRACE("myLevel_001::~myLevel_001()");
	}

	// dual creation allows for better error handling
	bool myLevel_001::create(KLS_Application* app, int id)
	{
		// log this activity
		KLS_TRACE("myLevel_001::create()");

		// call the base class
		KLS_CHECK(KLS_Level::create(app, id), "base class creation failed", KLS_FAILURE);

		// create a camera
		m_Camera = new KLS_Camera_FPS(getDriver(),getKeyMap_Arrows());

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool myLevel_001::cleanup()
	{
		// log this activity
		KLS_TRACE("myLevel_001::cleanup()");

		// safely delete the camera
		if (m_Camera) delete(m_Camera); m_Camera = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_Level::cleanup();
	}

	void myLevel_001::load()
	{
		// log this activity
		KLS_TRACE("myLevel_001::load()");

		// set the active camera
		if (m_Camera) getDriver()->setActiveCamera(m_Camera);
		getDriver()->setAmbientIntensity(0.1f);

		createWorld();

		// call the base class
		KLS_Level::load();
	}

	void myLevel_001::unload()
	{
		// log this activity
		KLS_TRACE("myLevel_003::unload()");

		destroyWorld();

		// call the base class
		KLS_Level::unload();
	}

	void myLevel_001::preFrame(const double& elapsedtime)
	{
		KLS_Level::preFrame(elapsedtime);
	}

	bool myLevel_001::onRMousePressed(const KLS_Event& e)
	{
		if (m_Camera) m_Camera->setActive(!m_Camera->getActive());
		// let the event die
		return KLS_SUCCESS;
	}

	bool myLevel_001::onLMousePressed(const KLS_Event& e)
	{
		// let the event die
		return KLS_SUCCESS;
	}

	// event overrides to provide functionality
	bool myLevel_001::onKeyDown(const KLS_Event& e)
	{
		return KLS_Level::onKeyDown(e);
	}

	// imgui related methods
	void myLevel_001::ImGui_draw()
	{
		// call the base class
		KLS_Level::ImGui_draw();
	}

	void myLevel_001::createWorld()
	{
		// create a few visuals
		KLS_Entity skybox = createSkybox();
		for (int x = 0; x < 1; x++)
		{
			KLS_SolarSystem solarsystem(this, KLS_SolarSystemInfo("SolarSystem", KLS_Transform(glm::vec3(x * -100.0f, -25.0f, 0.0f))));
		}
	}

	void myLevel_001::destroyWorld()
	{
		getECS()->clear();
	}

	KLS_Entity myLevel_001::createSkybox()
	{
		if (!getDriver()->getResourceManager()->getCubemap("skybox"))
		{
			std::string path = "../../../../_media/_Assets/_Textures/_skybox/space/purple/";
			getDriver()->getResourceManager()->createCubemap("skybox", false, path + "right.png", path + "left.png", path + "top.png", path + "bottom.png", path + "front.png", path + "back.png");
		}

		KLS_Mesh* Mesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/skybox.obj", KLSVT3D);
			Mesh->getMaterial(0).Cubemap[0] = "skybox";
			Mesh->getMaterial(0).MaterialType = KLSMT_SKYBOX;
		KLS_Entity e = getECS()->createEntity(getUniqueId(), "Skybox", KLS_Transform());
			e.addComponent<KLS_COMPONENT_SKYBOX>(Mesh);
		return e;
	}

} // end namespace