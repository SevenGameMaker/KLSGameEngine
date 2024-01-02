// include the needed header files
#include "_KLS_Driver.h"
#include "KLS_Logger.h"
#include "KLS_Window.h"
#include "KLS_Application.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_Camera.h"
#include "KLS_UniformData.h"
#include "KLS_Buffer.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_Driver::KLS_Driver() : KLS_EventHandler()
	{
		// log this activity
		KLS_TRACE("KLS_Driver::KLS_Driver()");
	}

	KLS_Driver::~KLS_Driver()
	{
		// log this activity
		KLS_TRACE("KLS_Driver::~KLS_Driver()");
	}

	// dual creation allows for better error handling
	bool KLS_Driver::create(KLS_Application* app)
	{
		// log this activity
		KLS_TRACE("KLS_Driver::create()");

		// remember this
		m_Application = app;
		m_Window = app->getWindow();

		m_ResourceManager = new KLS_ResourceManager();
		KLS_CHECK(m_ResourceManager->create(app), "resourcemanager creation failed", KLS_FAILURE);

		// everything went fine
		return KLS_SUCCESS;
	}

	// final creation method
	bool KLS_Driver::postCreate()
	{
		m_FrameBufferManager = new KLS_FrameBufferManager();
		m_FrameBufferManager->initialize();
		KLS_CHECK(m_FrameBufferManager->create(this), "framebuffermanager creation failed", KLS_FAILURE);

		// load up the default resources
		getResourceManager()->loadDefaults();

		// create the buffers
		m_UniformBufferData = instantiateUniformBuffer(sizeof(KLS_UniformData));
		m_UniformBufferLights = instantiateUniformBuffer(KLS_UniformDataLights::getSize());

		// make sure we have a default camera
		m_DefaultCamera = new KLS_Camera(this);

		// first time setup
		setClearColor(m_ClearColor);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_Driver::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_Driver::cleanup()");

		// safely delete the instance
		if (m_DefaultCamera) delete(m_DefaultCamera); m_DefaultCamera = nullptr;
		if (m_UniformBufferData) delete(m_UniformBufferData); m_UniformBufferData = nullptr;
		if (m_UniformBufferLights) delete(m_UniformBufferLights); m_UniformBufferLights = nullptr;

		// safely cleanup and delete the instance
		KLS_SAFE_CLEANUPANDDELETE(m_FrameBufferManager);
		KLS_SAFE_CLEANUPANDDELETE(m_ResourceManager);
	
		// forget this
		m_Window = nullptr;
		m_Application = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	bool KLS_Driver::onSystemWindowMoved(const KLS_Event& e)
	{
		return KLS_EventHandler::onSystemWindowMoved(e);
	}

	bool KLS_Driver::onSystemWindowSized(const KLS_Event& e)
	{
		return KLS_EventHandler::onSystemWindowSized(e);
	}

	bool KLS_Driver::onSystemWindowFocused(const KLS_Event& e)
	{
		return KLS_EventHandler::onSystemWindowFocused(e);
	}

	bool KLS_Driver::onSystemWindowIconified(const KLS_Event& e)
	{
		return KLS_EventHandler::onSystemWindowIconified(e);
	}

	bool KLS_Driver::onSystemWindowMaximize(const KLS_Event& e)
	{
		return KLS_EventHandler::onSystemWindowMaximize(e);
	}

	bool KLS_Driver::onSystemFrameBufferResized(const KLS_Event& e)
	{
		m_FrameBufferManager->onSystemFrameBufferResized();
		setViewport(m_Window->getWidth(), m_Window->getHeight());
		return KLS_EventHandler::onSystemFrameBufferResized(e);
	}


} // end namespace