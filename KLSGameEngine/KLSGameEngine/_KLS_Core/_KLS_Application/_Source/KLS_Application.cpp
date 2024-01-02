// include the needed header files
#include "KLS_Application.h"
#include "KLS_Logger.h"
#include "KLS_Window.h"
#include "_KLS_Driver.h"
#include "KLS_ImGui.h"
#include "KLS_Timer.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_LevelManager.h"
#include "KLS_Level.h"
#include "_KLS_PhysX.h"
#include "KLS_File.h"
#include "KLS_Editor.h"

// temproary use 
#include "_KLS_OpenGL_Driver.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_Application::KLS_Application()
	{
		// log this activity
		KLS_TRACE("KLS_Application::KLS_Application()");
	}

	KLS_Application::~KLS_Application()
	{
		// log this activity
		KLS_TRACE("KLS_Application::~KLS_Application()");
	}

	// dual creation allows for better error handling
	bool KLS_Application::create(KLS_ApplicationCreationInfo& info)
	{
		// log this activity
		KLS_TRACE("KLS_Application::create()");

		// cpoy the needed dll files into the working directory
		setupKLSSystems();

		// create the timer
		m_Timer = new KLS_Timer();
		m_Timer->pause();

		// create the instance
		m_Window = new KLS_Window();
		KLS_CHECK(m_Window->create(this, info),"window creation failed",KLS_FAILURE);

		// create the instance
		m_Driver = new KLS_OpenGL_Driver();
		KLS_CHECK(m_Driver->create(this), "driver creation failed", KLS_FAILURE);
		KLS_CHECK(m_Driver->postCreate(), "driver postcreation failed", KLS_FAILURE);

		// create the instance
		m_LevelManager = new KLS_LevelManager();
		KLS_CHECK(m_LevelManager->create(this), "levelmanager creation failed", KLS_FAILURE);

		// create the physx manager
		m_PhysX = new KLS_PhysX();
		m_PhysX->initialize();
		KLS_CHECK(m_PhysX->create(KLS_USE_GPU, KLS_USE_DEBUGRENDER, KLS_USE_PVD), "PhysX creation failed", KLS_FAILURE);

		// create the instance
		m_ImGui = new KLS_ImGui();
		KLS_CHECK(m_ImGui->create(this), "ImGui creation failed", KLS_FAILURE);

		// update the window
		getWindow()->forceResizeEvent();

		m_Editor = new KLS_Editor();
			m_Editor->create(this, KLS_ID_EDITOR);
			addLevel(m_Editor);

		// it seems we are ready to run
		setMode(KLS_MODE_RUN);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_Application::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_Application::cleanup()");

		// safely cleanup and delete the instance
		KLS_SAFE_CLEANUPANDDELETE(m_LevelManager);
		KLS_SAFE_CLEANUPANDDELETE(m_PhysX);
		KLS_SAFE_CLEANUPANDDELETE(m_ImGui);
		KLS_SAFE_CLEANUPANDDELETE(m_Driver);
		KLS_SAFE_CLEANUPANDDELETE(m_Window);

		// delete the timer
		if (m_Timer) delete(m_Timer); m_Timer = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_Application::preFrame(const double& elapsedtime)
	{
		if (m_LevelManager) m_LevelManager->preFrame(elapsedtime);
	}

	void KLS_Application::frame(const double& elapsedtime)
	{
		if (m_LevelManager) m_LevelManager->frame(elapsedtime);
	}

	void KLS_Application::render()
	{
		if (m_LevelManager) m_LevelManager->render();
	}

	// level related methods
	void KLS_Application::addLevel(KLS_Level* level)
	{
		if (m_LevelManager) m_LevelManager->addLevel(level);
	}

	void KLS_Application::destroyLevel(int id)
	{
		if (m_LevelManager) m_LevelManager->destroyLevel(id);
	}

	void KLS_Application::setNextLevelId(int id)
	{
		if (m_LevelManager) m_LevelManager->setNextLevelId(id);
	}

	KLS_Level* KLS_Application::getActiveLevel()
	{
		if (m_LevelManager) return m_LevelManager->getActiveLevel();
		return nullptr;
	}

	bool KLS_Application::onEvent(const KLS_Event& e)
	{
		if (getActiveLevel() && getActiveLevel()->onEvent(e)) return KLS_SUCCESS;
		return KLS_EventHandler::onEvent(e);
	}

	bool KLS_Application::onSystemClose(const KLS_Event& e)
	{
		// set the mode appropriately
		m_Mode = KLS_MODE_QUIT;

		// let the event continue
		return KLS_EventHandler::onSystemClose(e);
	}

	bool KLS_Application::onSystemWindowMoved(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemWindowMoved(e);

		// let the event continue
		return KLS_EventHandler::onSystemWindowMoved(e);
	}

	bool KLS_Application::onSystemWindowSized(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemWindowSized(e);

		// let the event continue
		return KLS_EventHandler::onSystemWindowSized(e);
	}

	bool KLS_Application::onSystemWindowFocused(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemWindowFocused(e);

		// let the event continue
		return KLS_EventHandler::onSystemWindowFocused(e);
	}

	bool KLS_Application::onSystemWindowIconified(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemWindowIconified(e);

		// let the event continue
		return KLS_EventHandler::onSystemWindowIconified(e);
	}

	bool KLS_Application::onSystemWindowMaximize(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemWindowMaximize(e);

		// let the event continue
		return KLS_EventHandler::onSystemWindowMaximize(e);
	}

	bool KLS_Application::onSystemFrameBufferResized(const KLS_Event& e)
	{
		// let the driver respond first
		if (getDriver()) getDriver()->onSystemFrameBufferResized(e);

		// let the event continue
		return KLS_EventHandler::onSystemFrameBufferResized(e);
	}

	bool KLS_Application::onSystemMouseEnter(const KLS_Event& e) 
	{ 
		setMode(KLS_MODE_RESUME);

		// let the event continue
		return KLS_EventHandler::onSystemMouseEnter(e);
	}

	bool KLS_Application::onSystemMouseExit(const KLS_Event& e) 
	{ 
		setMode(KLS_MODE_PAUSE);

		// let the event continue
		return KLS_EventHandler::onSystemMouseExit(e);
	}

	// imgui related methods
	void KLS_Application::ImGui_draw()
	{
		m_LevelManager->ImGui_draw();
		//if (m_RenderStats) m_ImGui->RenderAppStats();
		//m_ImGui->RenderFrameBuffers();
	}

	// set the running mode
	void KLS_Application::setMode(KLS_MODE mode)
	{
		if (mode == KLS_MODE_RESUME) m_Mode = KLS_MODE_RUN;
		else m_Mode = mode;
	}

	// primary application method. controls entire program flow
	void KLS_Application::run()
	{
		// log this activity
		KLS_INFO("Entering application run()");

		// resume the timer here to reset the totaltime
		m_Timer->resume();

		while (m_Mode != KLS_MODE_QUIT)
		{
			// poll for input
			m_Window->pollEvents();

			// tick the timer
			double elapsedtime = m_Timer->tick();

			// if we want to prevent a runaway framerate then clip it here
			if (getDriver()->getGPUProtect())
			{
				// protect the graphics card from running away
				if (m_Timer->getFrameRate() > getDriver()->getGPUMaxFrameRate()) 
					m_Window->setVSync(KLSVS_PROTECTED);
			}
			// let the framerate do whatever it can
			else m_Window->setVSync(KLSVS_UNLIMITED);


			switch (m_Mode)
			{
			case KLS_MODE_RUN:
			{
				// setup the framebuffers, clear the buffers etc...
				m_Driver->beginFrame();

				// internal frame methods that the user can override to provide functionality
				// level->ecs preframe
				preFrame(elapsedtime);

				// 
				m_Driver->frame(elapsedtime);

				// level->ecs frame
				frame(elapsedtime);

				// render the scene
				render();

				// end the frame 
				m_Driver->endFrame();

				// render the gui
				m_ImGui->beginFrame();
				ImGui_draw();
				m_ImGui->endFrame();

				// swap the window buffers
				m_Driver->swapBuffers();

			}	break;
			
			default:
				// setup the framebuffers, clear the buffers etc...
				m_Driver->beginFrame();

				// resolve the framebuffers
				m_Driver->frame(elapsedtime);

				// render the gui
				m_ImGui->beginFrame();
				ImGui_draw();
				m_ImGui->endFrame();

				// end the frame and swap the window buffers
				m_Driver->endFrame();
				break;
			}
		}

		// log this activity
		KLS_INFO("Exiting application run()");
	}

} // end namespace