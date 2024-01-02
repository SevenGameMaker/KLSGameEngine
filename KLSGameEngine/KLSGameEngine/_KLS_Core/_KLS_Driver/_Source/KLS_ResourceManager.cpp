// include the needed header files
#include "KLS_ResourceManager.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"


// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_ResourceManager::KLS_ResourceManager()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::KLS_ResourceManager()");
	}

	KLS_ResourceManager::~KLS_ResourceManager()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::~KLS_ResourceManager()");
	}

	// dual creation allows for better error handling
	bool KLS_ResourceManager::create(KLS_Application* app)
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::create()");

		// rememebr this
		m_Application = app;
		m_Driver = m_Application->getDriver();

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_ResourceManager::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_ResourceManager::cleanup()");

		// safely destroy the isntances
		removeAllCubemaps();
		removeAllTextures();

		// forget this
		m_Driver = nullptr;
		m_Application = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}


} // end namespace