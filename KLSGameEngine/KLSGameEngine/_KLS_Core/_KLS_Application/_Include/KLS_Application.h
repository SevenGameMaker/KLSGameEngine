// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_EventHandler.h"
#include "KLS_ApplicationCreationInfo.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Window KLS_Window;
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_ImGui KLS_ImGui;
	typedef class KLS_Timer KLS_Timer;
	typedef class KLS_LevelManager KLS_LevelManager;
	typedef class KLS_Level KLS_Level;
	typedef class KLS_PhysX KLS_PhysX;
	typedef class KLS_Editor KLS_Editor;

	/*
	* 
	*/
	class KLS_API KLS_Application : public KLS_EventHandler
	{
	private:
		KLS_VARIABLE_GET(KLS_MODE, Mode, KLS_MODE::KLS_MODE_INIT);
		KLS_VARIABLE_GET(KLS_Window*, Window, nullptr);
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_GET(KLS_ImGui*, ImGui, nullptr);
		KLS_VARIABLE_GET(KLS_Timer*, Timer, nullptr);
		KLS_VARIABLE_GET(KLS_LevelManager*, LevelManager, nullptr);
		KLS_VARIABLE_GET(KLS_PhysX*, PhysX, nullptr);				// the physx wrapper

		KLS_VARIABLE_SETGET(bool, RenderStats, true);
		KLS_VARIABLE_GET(KLS_Editor*, Editor, nullptr);

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_Application();
		virtual ~KLS_Application();

		// dual creation allows for better error handling
		virtual bool create(KLS_ApplicationCreationInfo& info);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// frame methods
		virtual void preFrame(const double& elapsedtime);
		virtual void frame(const double& elapsedtime);
		virtual void render();

		// primary application method. controls entire program flow
		virtual void run();

		// set the running mode
		virtual void setMode(KLS_MODE mode);

		// event overrides to provide functionality
		virtual bool onEvent(const KLS_Event& e);
		virtual bool onSystemClose(const KLS_Event& e);
		virtual bool onSystemWindowMoved(const KLS_Event& e);
		virtual bool onSystemWindowSized(const KLS_Event& e);
		virtual bool onSystemWindowFocused(const KLS_Event& e);
		virtual bool onSystemWindowIconified(const KLS_Event& e);
		virtual bool onSystemWindowMaximize(const KLS_Event& e);
		virtual bool onSystemFrameBufferResized(const KLS_Event& e);
		virtual bool onSystemMouseEnter(const KLS_Event& e);
		virtual bool onSystemMouseExit(const KLS_Event& e);

		// imgui related methods
		virtual void ImGui_draw();

		// level related methods
		void addLevel(KLS_Level* level);
		void destroyLevel(int id);
		void setNextLevelId(int id);
		KLS_Level* getActiveLevel();
	};


} // end namespace