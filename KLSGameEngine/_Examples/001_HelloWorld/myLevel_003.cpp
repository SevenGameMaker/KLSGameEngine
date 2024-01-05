// include the needed header files
#include "myLevel_003.h"
#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_ImGui.h"

#include "KLS_Camera_FPS.h"
#include "KLS_ECS.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include "_KLS_ECS_Systems.h"
#include "KLS_Math.h"
#include "KLS_PhysX_World.h"
#include "KLS_PhysX_Object.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	myLevel_003::myLevel_003() : KLS_Level()
	{
		// log this activity
		KLS_TRACE("myLevel_003::myLevel_003()");
	}

	myLevel_003::~myLevel_003()
	{
		// log this activity
		KLS_TRACE("myLevel_003::~myLevel_003()");
	}

	// dual creation allows for better error handling
	bool myLevel_003::create(KLS_Application* app, int id)
	{
		// log this activity
		KLS_TRACE("myLevel_003::create()");

		// call the base class
		KLS_CHECK(KLS_Level::create(app, id), "base class creation failed", KLS_FAILURE);

		// create a fps camera
		m_Camera = new KLS_Camera_FPS(getDriver(), getKeyMap_WASD());
			m_Camera->setPosition(glm::vec3(-10, 10, -10));
			m_Camera->setTarget(glm::vec3(0, 0, 0));

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool myLevel_003::cleanup()
	{
		// log this activity
		KLS_TRACE("myLevel_003::cleanup()");

		// safely delete the camera
		if (m_Camera) delete(m_Camera); m_Camera = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_Level::cleanup();
	}

	void myLevel_003::load()
	{
		// log this activity
		KLS_TRACE("myLevel_003::load()");

		// set the active camera
		if (m_Camera) getDriver()->setActiveCamera(m_Camera);
		getDriver()->setAmbientIntensity(0.1f);

		createWorld();

		// call the base class
		KLS_Level::load();
	}

	void myLevel_003::unload()
	{
		// log this activity
		KLS_TRACE("myLevel_003::unload()");

		destroyWorld();

		// call the base class
		KLS_Level::unload();
	}

	void myLevel_003::createWorld()
	{
		KLS_Entity le = getECS()->createEntity(getUniqueId(), "Light", KLS_Transform(glm::vec3(0,5,0), KLS_IDENTITY_QUAT, glm::vec3(1.f)));
		KLS_LightData ld;
			ld.color = getRandomColor();
			le.addComponent<KLS_COMPONENT_LIGHT>(ld);
		KLS_Mesh* lightMesh = getDriver()->getResourceManager()->getMesh("../../../../_media/_assets/_models/_default/light.obj", KLSVT3D);
			lightMesh->getMaterial(0).ColorDiffuse = ld.color;
			lightMesh->getMaterial(0).MaterialType = KLSMT_COLOR;
			le.addComponent<KLS_COMPONENT_MESH>(lightMesh);

			// Add the circular orbit component for the light
		KLS_COMPONENT_FLYCIRCLE fc;
			fc.Center = glm::vec3(0.0f,5.0f,0.0f);
			fc.Radius = 20.0f;
			fc.Speed = 1.0f;
			fc.StartTime = 0.0f;
		le.addComponent<KLS_COMPONENT_FLYCIRCLE>(fc);

		KLS_Entity plane = createCube(KLS_Entity(), getUniqueId(), KLS_Transform(glm::vec3(0), KLS_IDENTITY_QUAT, glm::vec3(100,1,100)), true, false, glm::vec3(), 0.0f);
	}

	void myLevel_003::destroyWorld()
	{
		getECS()->clear();
	}

	bool myLevel_003::onLMousePressed(const KLS_Event& e)
	{
		KLS_Entity cube = createCube(KLS_Entity(), getUniqueId(), KLS_Transform(m_Camera->getTransform().getPosition(), KLS_IDENTITY_QUAT, glm::vec3(4)),
			true, true, m_Camera->getIn(), 100.0f);

		// let the event die
		return KLS_SUCCESS;
	}

	bool myLevel_003::onRMousePressed(const KLS_Event& e)
	{
		if (m_Camera) m_Camera->setActive(!m_Camera->getActive());

		// let the event die
		return KLS_SUCCESS;
	}

	bool myLevel_003::onMMousePressed(const KLS_Event& e)
	{
		setSelected(getSelectedObject(e.Mouse.X, e.Mouse.Y));
		getECS()->removeEntity(getSelected());
		// let the event die
		return KLS_SUCCESS;
	}

	// imgui related methods
	void myLevel_003::ImGui_draw()
	{
		// call the base class
		KLS_Level::ImGui_draw();
	}

} // end namespace