// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Event.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Application KLS_Application;
	typedef class KLS_Level KLS_Level;

	/*
	*
	*/
	class KLS_API KLS_LevelManager
	{
	private:
		KLS_VARIABLE_GET(KLS_Application*, Application, nullptr);
		KLS_VARIABLE_GET(KLS_Level*, ActiveLevel, nullptr);
		
		KLS_VARIABLE_SETGET(int, ActiveLevelId, 0);
		KLS_VARIABLE_SETGET(int, NextLevelId, 0);

		std::vector<KLS_Level*> m_Levels;

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_LevelManager();
		virtual ~KLS_LevelManager();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// frame methods
		virtual void preFrame(const double& elapsedtime);
		virtual void frame(const double& elapsedtime);
		virtual void render();

		// event overrides to provide functionality
		virtual bool onSystemFrameBufferResized(const KLS_Event& e);

		// imgui related methods
		virtual void ImGui_draw();

		virtual void addLevel(KLS_Level* level);
		virtual KLS_Level* getLevel(int id);
		virtual void destroyLevel(int id);
		virtual void destroyAllLevels();

		virtual void levelChange();
	};

} // end namespace
