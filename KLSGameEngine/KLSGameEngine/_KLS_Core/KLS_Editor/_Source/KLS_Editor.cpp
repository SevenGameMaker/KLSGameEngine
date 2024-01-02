// include the needed header files
#include "KLS_Editor.h"
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
#include <imgui.h>
#include <imguizmo.h>

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_Editor::KLS_Editor() : KLS_Level()
	{
		// log this activity
		KLS_TRACE("KLS_Editor::KLS_Editor()");
	}

	KLS_Editor::~KLS_Editor()
	{
		// log this activity
		KLS_TRACE("KLS_Editor::~KLS_Editor()");
	}

	// dual creation allows for better error handling
	bool KLS_Editor::create(KLS_Application* app, int id)
	{
		// log this activity
		KLS_TRACE("KLS_Editor::create()");

		// call the base class
		KLS_CHECK(KLS_Level::create(app, id), "base class creation failed", KLS_FAILURE);

		// create a camera
		m_Camera = new KLS_Camera_FPS(getDriver(), getKeyMap_WASD());
		m_Camera->setPosition(glm::vec3(-30, 40, -30));
		m_Camera->setTarget(glm::vec3(0, 20, 0));

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_Editor::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_Editor::cleanup()");

		// safely delete the camera
		if (m_Camera) delete(m_Camera); m_Camera = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_Level::cleanup();
	}

	void KLS_Editor::load()
	{
		// log this activity
		KLS_TRACE("KLS_Editor::load()");

		// set the active camera
		if (m_Camera) getDriver()->setActiveCamera(m_Camera);
		getDriver()->setAmbientIntensity(0.1f);

		createDefaultWorld();

		// call the base class
		KLS_Level::load();
	}

	void KLS_Editor::unload()
	{
		// log this activity
		KLS_TRACE("myLevel_003::unload()");

		destroyWorld();

		// call the base class
		KLS_Level::unload();
	}

	void KLS_Editor::preFrame(const double& elapsedtime)
	{
		KLS_Level::preFrame(elapsedtime);
	}

	void KLS_Editor::frame(const double& elapsedtime)
	{
		KLS_Level::frame(elapsedtime);
	}

	void KLS_Editor::render()
	{
		getDriver()->getFrameBufferManager()->setCurrentFBO("imguifbo", KLSCF_ALL);
		KLS_Level::render();
		getDriver()->getFrameBufferManager()->setCurrentFBO("fbo0", KLSCF_ALL);
	}

	bool KLS_Editor::onLMousePressed(const KLS_Event& e)
	{
		if ((!ImGuizmo::IsUsing()) && (!ImGuizmo::IsOver()) && isPointInsideRect(m_WindowPosition, glm::vec2(e.Mouse.X, e.Mouse.Y)))
		{
			KLS_UUID id;
			int meshbuffer;
			getDriver()->getFrameBufferManager()->getFrameBuffer("imguifbo")->getObjectIdFromColorBuffer(
				KLS_EventHandler::MouseX() - static_cast<int>(m_WindowPosition.x), KLS_EventHandler::MouseY() - static_cast<int>(m_WindowPosition.y), 1,
				id, meshbuffer);

			setSelected(id, meshbuffer);
		}

		// let the base class handle the event
		return KLS_Level::onLMousePressed(e);
	}

	bool KLS_Editor::onMMousePressed(const KLS_Event& e)
	{
		if (isPointInsideRect(m_WindowPosition, glm::vec2(e.Mouse.X, e.Mouse.Y)))
		{
			KLS_Entity cube = createCube(KLS_Entity(), getUniqueId(), KLS_Transform(m_Camera->getTransform().getPosition(), KLS_IDENTITY_QUAT, glm::vec3(1)),
				true, true, m_Camera->getIn(), 100.0f);
			cube.addComponent<KLS_COMPONENT_DEATHTIMER>(5.0f);
		}

		// let the base class handle the event
		return KLS_Level::onMMousePressed(e);
	}

	bool KLS_Editor::onRMousePressed(const KLS_Event& e)
	{
		if (isPointInsideRect(m_WindowPosition, glm::vec2(e.Mouse.X, e.Mouse.Y)))
		{
			if (m_Camera) m_Camera->setActive(!m_Camera->getActive());
		}

		// let the base class handle the event
		return KLS_Level::onRMousePressed(e);
	}

	// event overrides to provide functionality
	bool KLS_Editor::onKeyDown(const KLS_Event& e)
	{
		switch (e.Keyboard.Key)
		{
		case KLS_KEYCODE::KEY_DELETE:
		{
			if (!m_Selected.isNull())
			{
				getECS()->removeEntity(m_Selected);
				setSelected(0,0);
				return true;
			}
		} break;
		case KLS_KEYCODE::KEY_1: { m_Operation = ImGuizmo::OPERATION::TRANSLATE; return true; } break;
		case KLS_KEYCODE::KEY_2: { m_Operation = ImGuizmo::OPERATION::ROTATE; return true; } break;
		case KLS_KEYCODE::KEY_3: { m_Operation = ImGuizmo::OPERATION::SCALE; return true; } break;
		case KLS_KEYCODE::KEY_4: { m_Operation = ImGuizmo::OPERATION::UNIVERSAL; return true; } break;
		case KLS_KEYCODE::KEY_SPACE:
		{
			KLS_Entity cube = createCube(KLS_Entity(), getUniqueId(), KLS_Transform(m_Camera->getTransform().getPosition(), KLS_IDENTITY_QUAT, glm::vec3(1)),
				true, true, m_Camera->getIn(), 100.0f);
			cube.addComponent<KLS_COMPONENT_DEATHTIMER>(5.0f);
			return true;
		} break;
		}

		// let the base class handle the event
		return KLS_Level::onKeyDown(e);
	}

	void KLS_Editor::setSelected(KLS_UUID id, int meshbuffer)
	{
		// remove the hilite from the currently selected entity if it exists
		if (!m_Selected.isNull())
		{
			if (m_Selected.hasComponent<KLS_COMPONENT_HILITE>()) m_Selected.removeComponent<KLS_COMPONENT_HILITE>();
			if (m_Selected.hasComponent<KLS_COMPONENT_DEBUG>()) m_Selected.removeComponent<KLS_COMPONENT_DEBUG>();
		}

		m_Selected = getECS()->findEntityById(id);
		m_SelectedMeshBuffer = meshbuffer;

		if (!m_Selected.isNull())
		{
			m_Origin = m_Selected.getTransform();
			if (m_ShowSelectedBuffer) m_Selected.addComponent<KLS_COMPONENT_HILITE>(meshbuffer);
			if (m_ShowDebugInfo) m_Selected.addComponent<KLS_COMPONENT_DEBUG>(true, nullptr);
		}
		else
		{
			m_Origin = KLS_Transform();
		}
	}

	// imgui related methods
	void KLS_Editor::ImGui_draw()
	{
		// call the base class
		KLS_Level::ImGui_draw();

		drawMainWindow();
	}

	void KLS_Editor::createDefaultWorld()
	{

#if(0)		
		// create a few visuals
		m_Skybox = createSkybox("editorskybox", "../../../../_media/_Assets/_Textures/_skybox/space/purple/");
		m_Grid = createGrid();

		KLS_Transform t(glm::vec3(0, -5.01, 0), KLS_IDENTITY_QUAT, glm::vec3(200, 10, 200));
		createCube(KLS_Entity(), getUniqueId(), t, true, false, glm::vec3(0), 0);

		createStackOfCubes(glm::vec3(20, 5, 20), glm::vec3(2), 5, true, true);
		createPyramidOfCubes(glm::vec3(0, 1, 0), 7, 7, 2, true, true);
#endif
		m_Light = createLight(KLS_Entity(), getUniqueId(), KLS_Transform(glm::vec3(0, 10, 0), KLS_IDENTITY_QUAT, glm::vec3(1)), KLS_WHITE);
		createMesh(KLS_Entity(), getUniqueId(), KLS_Transform(glm::vec3(-20, 1, -20), KLS_IDENTITY_QUAT, glm::vec3(.1f)),
			"../../../../_Media/_Assets/_Models/___Converted/_Buildings/Castle/castle.obj",
			KLSMT_TEXTURED, true);
	}

	void KLS_Editor::destroyWorld()
	{
		getECS()->clear();
	}

	void KLS_Editor::setShowDebugInfo(bool value)
	{
		if (value == m_ShowDebugInfo) return;
		m_ShowDebugInfo = value;

		if (!m_Selected.isNull())
		{
			if ((!m_ShowDebugInfo) && (m_Selected.hasComponent<KLS_COMPONENT_DEBUG>()))
				m_Selected.removeComponent<KLS_COMPONENT_DEBUG>();
			if (m_ShowDebugInfo)
				m_Selected.addComponent<KLS_COMPONENT_DEBUG>(true, nullptr);
		}
	}

	void KLS_Editor::setShowSelectedBuffer(bool value)
	{
		if (value == m_ShowSelectedBuffer) return;
		m_ShowSelectedBuffer = value;

		if (!m_Selected.isNull())
		{
			if ((!m_ShowSelectedBuffer) && (m_Selected.hasComponent<KLS_COMPONENT_HILITE>()))
				m_Selected.removeComponent<KLS_COMPONENT_HILITE>();
			if (m_ShowSelectedBuffer)
				m_Selected.addComponent<KLS_COMPONENT_HILITE>(m_SelectedMeshBuffer);
		}
	}

	void KLS_Editor::toggleGrid()
	{
		if (m_Grid.isNull()) m_Grid = createGrid();
		else
		{
			getECS()->removeEntity(m_Grid);
		}
	}

	void KLS_Editor::toggleSkybox()
	{
		if (m_Skybox.isNull()) m_Skybox = createSkybox("editorskybox", "../../../../_media/_Assets/_Textures/_skybox/space/purple/");
		else
		{
			getECS()->removeEntity(m_Skybox);
		}
	}

} // end namespace