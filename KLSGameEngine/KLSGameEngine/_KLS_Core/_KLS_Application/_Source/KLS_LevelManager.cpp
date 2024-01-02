// include the needed header files
#include "KLS_LevelManager.h"
#include "KLS_Logger.h"
#include "KLS_Application.h"
#include "KLS_Level.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_LevelManager::KLS_LevelManager()
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::KLS_LevelManager()");
	}

	KLS_LevelManager::~KLS_LevelManager()
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::~KLS_LevelManager()");
	}

	// dual creation allows for better error handling
	bool KLS_LevelManager::create(KLS_Application* app)
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::create()");

		// remember this
		m_Application = app;

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_LevelManager::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::cleanup()");

		// safely destroy all of the levels
		destroyAllLevels();

		// forget this
		m_Application = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_LevelManager::preFrame(const double& elapsedtime)
	{
		levelChange();
		if (m_ActiveLevel) m_ActiveLevel->preFrame(elapsedtime);
	}

	void KLS_LevelManager::frame(const double& elapsedtime)
	{
		if (m_ActiveLevel) m_ActiveLevel->frame(elapsedtime);
	}

	void KLS_LevelManager::render()
	{
		if (m_ActiveLevel) m_ActiveLevel->render();
	}

	bool KLS_LevelManager::onSystemFrameBufferResized(const KLS_Event& e)
	{
		// let the event continue
		return KLS_FAILURE;
	}

	// imgui related methods
	void KLS_LevelManager::ImGui_draw()
	{
		if (m_ActiveLevel) m_ActiveLevel->ImGui_draw();
	}

	void KLS_LevelManager::addLevel(KLS_Level* level)
	{
		m_Levels.push_back(level);
	}

	KLS_Level* KLS_LevelManager::getLevel(int id)
	{
		if (m_Levels.empty()) return nullptr;
		std::vector<KLS_Level*>::iterator it;
		for (it = m_Levels.begin(); it != m_Levels.end(); it++)
		{
			if ((*it)->getId() == id) return (*it);
		}
		return nullptr;
	}

	void KLS_LevelManager::destroyLevel(int id)
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::destroyLevel({})", id);

		if (m_Levels.empty()) return;
		std::vector<KLS_Level*>::iterator it;
		for (it = m_Levels.begin(); it != m_Levels.end(); it++)
		{
			if ((*it)->getId() == id)
			{
				(*it)->cleanup();
				delete((*it));
				m_Levels.erase(it);
				return;
			}
		}
	}

	void KLS_LevelManager::destroyAllLevels()
	{
		// log this activity
		KLS_TRACE("KLS_LevelManager::destroyAllLevels()");

		if (m_Levels.empty()) return;
		std::vector<KLS_Level*>::iterator it;
		for (it = m_Levels.begin(); it != m_Levels.end();)
		{
			(*it)->cleanup();
			delete((*it));
			it = m_Levels.erase(it);
		}
	}

	void KLS_LevelManager::levelChange()
	{
		// if we need to change levels
		if (m_ActiveLevelId != m_NextLevelId)
		{
			if (m_ActiveLevel) m_ActiveLevel->unload();
			m_ActiveLevel = nullptr;

			m_ActiveLevel = getLevel(m_NextLevelId);
			if (m_ActiveLevel)
			{
				m_ActiveLevelId = m_NextLevelId;
				m_ActiveLevel->load();
			}
			m_NextLevelId = m_ActiveLevelId;
		}
	}

} // end namespace