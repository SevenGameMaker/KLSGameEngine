// include the needed header files
#include "KLS_Level.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"
#include "KLS_ImGui.h"
#include "KLS_ECS.h"
#include "_KLS_ECS_Systems.h"
#include "KLS_Entity.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_UniformData.h"
#include "_KLS_ECS_Components.h"
#include "KLS_PhysX_World.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_Level::KLS_Level() : KLS_EventHandler()
	{
		// log this activity
		KLS_TRACE("KLS_Level::KLS_Level()");
	}

	KLS_Level::~KLS_Level()
	{
		// log this activity
		KLS_TRACE("KLS_Level::~KLS_Level()");
	}

	// dual creation allows for better error handling
	bool KLS_Level::create(KLS_Application* app, int id)
	{
		// log this activity
		KLS_TRACE("KLS_Level::create()");

		// remember this
		m_Application = app;
		m_Driver = m_Application->getDriver();
		m_Id = id;

		// create the physx world
		m_PhysXWorld = new KLS_PhysX_World();
		m_PhysXWorld->initialize();
		KLS_CHECK(m_PhysXWorld->create(this), "PhysXWorld creation failed", KLS_FAILURE);

		// create the ECS
		m_ECS = new KLS_ECS(this);
			
			// preframe systems
			getECS()->addpreFrameSystem(new KLS_ECS_System_Controller(m_ECS));

			// frame systems
			getECS()->addframeSystem(new KLS_ECS_System_DeathTimer(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_PhysX(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_Rotate(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_FlyCircle(m_ECS));

			getECS()->addframeSystem(new KLS_ECS_System_CameraDistance(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_Lights(m_ECS));

			getECS()->addframeSystem(new KLS_ECS_System_Skybox(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_Mesh(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_Debug(m_ECS));
			getECS()->addframeSystem(new KLS_ECS_System_Hilite(m_ECS));

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_Level::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_Level::cleanup()");

		// safely delete the instance
		if (m_ECS) delete(m_ECS); m_ECS = nullptr;

		KLS_SAFE_CLEANUPANDDELETE(m_PhysXWorld);

		// forget this
		m_Application = nullptr;
		m_Driver = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_Level::load()
	{
	}

	void KLS_Level::unload()
	{
	}

	void KLS_Level::updateLightUniforms()
	{
		// Sort the vector using a lambda expression as the comparator : front to back 
		std::sort(m_Lights.begin(), m_Lights.end(), [](KLS_RenderLight a, KLS_RenderLight& b)
			{ return a.m_LightData.cameraDistance < a.m_LightData.cameraDistance; });

		// fill in a KLS_UniformDataLights structure
		KLS_UniformDataLights lightData;
		for (int x = 0; x < m_Lights.size(); x++)
		{
			KLS_LightData l = m_Lights[x].m_LightData;
			lightData.lightColor[x] = l.color;
			lightData.lightPosition[x] = l.position;
			lightData.lightIntensity[x] = l.intensity;
			lightData.lightAttenuation[x] = l.attenuation;
		}
		getDriver()->setNumberOfLights(static_cast<int>(m_Lights.size()));

		// upload the KLS_UniformDataLights structure 
		getDriver()->getUniformBufferLights()->bind();
		getDriver()->getUniformBufferLights()->prepare(&lightData, KLS_UniformDataLights::getSize(), 1);
	}

	void KLS_Level::drawSkyboxes()
	{
		getDriver()->setDepth(false);
		getDriver()->setTransparency(false);
		for (uint32_t i = 0; i < m_Skyboxes.size(); i++)
		{
			getDriver()->drawMesh(m_Skyboxes[i].m_Mesh, m_Skyboxes[i].m_Transform.getModelMatrix());
		}
	}

	void KLS_Level::drawSolids()
	{
		// Sort the vector using a lambda expression as the comparator : front to back to help with overdraw
		std::sort(m_Solid.begin(), m_Solid.end(), [](KLS_RenderMesh a, KLS_RenderMesh& b)
			{ return a.m_CameraDistance < a.m_CameraDistance; });

		getDriver()->setDepth(true);
		getDriver()->setTransparency(false);
		for (uint32_t i = 0; i < m_Solid.size(); i++)
		{
			KLS_Camera* cam = getDriver()->getActiveCamera();
			getDriver()->drawMesh(m_Solid[i].m_Mesh, m_Solid[i].m_Transform.getModelMatrix());
		}
	}

	void KLS_Level::drawTransparent()
	{
		// Sort the vector using a lambda expression as the comparator : back to front to help with transparency
		std::sort(m_Transparent.begin(), m_Transparent.end(), [](KLS_RenderMesh a, KLS_RenderMesh& b)
			{ return a.m_CameraDistance > a.m_CameraDistance; });

		getDriver()->setTransparency(true);
		for (uint32_t i = 0; i < m_Transparent.size(); i++)
		{
			getDriver()->drawMesh(m_Transparent[i].m_Mesh, m_Transparent[i].m_Transform.getModelMatrix());
		}
	}

	void KLS_Level::preFrame(const double& elapsedtime)
	{
		if (m_PhysXWorld) m_PhysXWorld->preFrame(1.0f/60.0f);
		if (getECS()) getECS()->preFrame(elapsedtime);
	}

	void KLS_Level::frame(const double& elapsedtime)
	{
		m_Solid.clear();
		m_Transparent.clear();
		m_Lights.clear();
		m_Skyboxes.clear();
		if (getECS()) getECS()->frame(elapsedtime);
		updateLightUniforms();
	}

	void KLS_Level::render()
	{
		drawSkyboxes();
		drawSolids();
		drawTransparent();
	}

	bool KLS_Level::onSystemFrameBufferResized(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemFrameBufferResized(e);

		// let the event continue
		return KLS_EventHandler::onSystemFrameBufferResized(e);
	}

	// imgui related methods
	void KLS_Level::ImGui_draw()
	{
	}

	// select an object using pixel perfect selection
	KLS_Entity KLS_Level::getSelectedObject(int mousex, int mousey)
	{
		KLS_UUID id;
		int meshbuffer;
		getDriver()->getFrameBufferManager()->getFrameBuffer("fbo0")->getObjectIdFromColorBuffer(mousex,mousey, 1,id,meshbuffer);
		//KLS_ERROR("selected object with id = {}", objectId);
		return getECS()->findEntityById(id);
	}

	void KLS_Level::onMessage(const KLS_Message& msg)
	{
		switch (msg.Type)
		{
			case KLS_MT_COLLISION: { KLS_WARN("Message: KLS_MT_COLLISION {} {}", msg.Source, msg.Dest); } break;
			case KLS_MT_COLLISIONLOST: { KLS_WARN("Message: KLS_MT_COLLISIONLOST {} {}", msg.Source, msg.Dest); } break;
			case KLS_MT_CHARACTER_COLLISION: { KLS_WARN("Message: KLS_MT_CHARACTER_COLLISION {} {}", msg.Source, msg.Dest); } break;
			case KLS_MT_CHARACTER_COLLISIONLOST: { KLS_WARN("Message: KLS_MT_CHARACTER_COLLISIONLOST {} {}", msg.Source, msg.Dest); } break;
			default	: {KLS_WARN("Message: unknown type({}) {} {}", msg.Type, msg.Source, msg.Dest); }
		}
	}

} // end namespace