// include the needed header files
#include "myApp.h"
#include "KLS_Logger.h"
#include "KLS_ImGui.h"
#include <imgui.h>
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_Math.h"

#include "myLevel_001.h"
#include "myLevel_002.h"
#include "myLevel_003.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	myApp::myApp()
	{
		// log this activity
		KLS_TRACE("myApp::myApp()");
	}

	myApp::~myApp()
	{
		// log this activity
		KLS_TRACE("myApp::~myApp()");
	}

	// dual creation allows for better error handling
	bool myApp::create(KLS_ApplicationCreationInfo& info)
	{
		// log this activity
		KLS_TRACE("myApp::create()");

		// call the base class
		KLS_CHECK(KLS_Application::create(info), "base class creation failed", KLS_FAILURE);

#if(1)
		myLevel_001* l = new myLevel_001();
		l->create(this, 1);
		addLevel(l);
		myLevel_002* l2 = new myLevel_002();
		l2->create(this, 2);
		addLevel(l2);
		myLevel_003* l3 = new myLevel_003();
		l3->create(this, 3);
		addLevel(l3);
#endif

		setNextLevelId(KLS_ID_EDITOR);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool myApp::cleanup()
	{
		// log this activity
		KLS_TRACE("myApp::cleanup()");

		// let the base class cleanup it's own memory mess
		return KLS_Application::cleanup();
	}

	void myApp::preFrame(const double& elapsedtime)
	{
		KLS_Application::preFrame(elapsedtime);
	}

	void myApp::frame(const double& elapsedtime)
	{
		KLS_Application::frame(elapsedtime);
	}

	bool myApp::onKeyDown(const KLS_Event& e)
	{ 
		switch (e.Keyboard.Key)
		{
		case KLS_KEYCODE::KEY_ESCAPE: setMode(KLS_MODE_QUIT); return KLS_SUCCESS;; break;
		case KLS_KEYCODE::KEY_GRAVE_ACCENT: setNextLevelId(KLS_ID_EDITOR); return KLS_SUCCESS;; break;
		case KLS_KEYCODE::KEY_1: setNextLevelId(1); return KLS_SUCCESS;; break;
		case KLS_KEYCODE::KEY_2: setNextLevelId(2); return KLS_SUCCESS;; break;
		case KLS_KEYCODE::KEY_3: setNextLevelId(3); return KLS_SUCCESS;; break;
		}

		// let the event continue
		return KLS_Application::onKeyDown(e);
	}

	void myApp::ImGui_draw()
	{
		KLS_Application::ImGui_draw();
	}

} // end namespace