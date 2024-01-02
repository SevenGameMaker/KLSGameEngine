// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// include the specific header files
#include "KLS_Logger.h"
#include "KLS_Event.h"
#include "KLS_KeyCodes.h"
#include "KLS_Event.h"

// unclutter the global namespace
namespace KLS
{
	/*
	* parse events into functionc calls
	*/
	class KLS_API KLS_EventHandler
	{
	private:
		KLS_VARIABLE_SETGET(int, EventHandlerId, 0);						// id for logging purposes
		KLS_VARIABLE_SETGET(std::string, EventHandlerName, "EventHandler");	// name for logging purposes
	public:
		// Keyboard key states. basically a global keyboard map accessible anywhere in the program
		// i know, i know, global variables are bad.....
		// this stops us from having a keyboard map for each and every event (memory reduction)
		static keyStatesENUM m_Keyboard[KLS_KEYCODE::KEY_COUNT];

		// basically, global methods to retrieve keyboard states
		static bool IsKeyPressed(KLS_KEYCODE keycode) { return (m_Keyboard[keycode] == DOWN || m_Keyboard[keycode] == PRESSED); }
		static bool IsKeyUp(KLS_KEYCODE keycode) { return (m_Keyboard[keycode] == UP || m_Keyboard[keycode] == RELEASED); }

		// basically these are global variables that can be accessed anywhere in the program
		static int m_MouseX;
		static int m_MouseY;
		static bool m_LButton;
		static bool m_MButton;
		static bool m_RButton;

	public:
		// current mouse informtion getters
		static int	MouseX() { return m_MouseX; }
		static int	MouseY() { return m_MouseY; }
		static bool LButton() { return m_LButton; }
		static bool MButton() { return m_MButton; }
		static bool RButton() { return m_RButton; }

	public:
		// do nothing constructor / destructor (use initialize,create and destroy to manage lifespan)
		KLS_EventHandler();
		virtual ~KLS_EventHandler();

		// parse the event into individual function calls
		virtual bool onEvent(const KLS_Event& e);

		// handle system level events
		virtual bool onEventSystem(const KLS_Event& e);
		virtual bool onSystemClose(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemWindowMoved(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemWindowSized(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemWindowFocused(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemWindowIconified(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemWindowMaximize(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemFrameBufferResized(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }

		virtual bool onSystemMouseEnter(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemMouseExit(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemFilenameDrop(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }

		virtual bool onSystemLevelAdded(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemLevelRemoved(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemLevelPreChange(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onSystemLevelChanged(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }

		// handle mouse events. update the global mouse information
		virtual bool onEventMouse(const KLS_Event& e);
		virtual bool onLMousePressed(const KLS_Event& e) { m_LButton = true; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onMMousePressed(const KLS_Event& e) { m_MButton = true; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onRMousePressed(const KLS_Event& e) { m_RButton = true; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onLMouseReleased(const KLS_Event& e) { m_LButton = false; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onMMouseReleased(const KLS_Event& e) { m_MButton = false; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onRMouseReleased(const KLS_Event& e) { m_RButton = false; KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onMouseMoved(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onMouseWheel(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }

		// handle keyboard events. update the static keyboard  information
		virtual bool onEventKeyboard(const KLS_Event& e);
		virtual bool onKeyDown(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }
		virtual bool onKeyUp(const KLS_Event& e) { KLS_LOG_EVENT(e); return KLS_FAILURE; }

	};

} // end namespace
