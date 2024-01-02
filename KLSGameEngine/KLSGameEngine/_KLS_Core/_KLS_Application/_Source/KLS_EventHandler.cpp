// include the needed header files
#include "KLS_EventHandler.h"
#include "KLS_Application.h"

namespace KLS
{
	// static variables
	keyStatesENUM KLS_EventHandler::m_Keyboard[KLS_KEYCODE::KEY_COUNT];
	int KLS_EventHandler::m_MouseX;
	int KLS_EventHandler::m_MouseY;
	bool KLS_EventHandler::m_LButton;
	bool KLS_EventHandler::m_MButton;
	bool KLS_EventHandler::m_RButton;

	// class constructor
	KLS_EventHandler::KLS_EventHandler()
	{
		// log this activity
		KLS_TRACE("KLS_EventHandler::KLS_EventHandler()");

		// set all of the static information to default values
		for (int i = 0; i < KLS_KEYCODE::KEY_COUNT; i++) m_Keyboard[i] = RELEASED;
		m_MouseX	= 0;
		m_MouseY	= 0;
		m_LButton = false;
		m_MButton = false;
		m_RButton = false;
	}

	// class destructor
	KLS_EventHandler::~KLS_EventHandler()
	{
		// log this activity
		KLS_TRACE("KLS_EventHandler::~KLS_EventHandler()");
	}

	// parse the event into individual function calls
	bool KLS_EventHandler::onEvent(const KLS_Event& e)
	{
		switch (e.EventType)
		{
		case KLS_EVENTTYPE::KLS_EVT_SYSTEM: return onEventSystem(e); break;
		case KLS_EVENTTYPE::KLS_EVT_MOUSE:	return onEventMouse(e);	 break;
		case KLS_EVENTTYPE::KLS_EVT_KEYBOARD: return onEventKeyboard(e); break;
		}
		return KLS_FAILURE;
	}

	// parse the event into individual function calls
	bool KLS_EventHandler::onEventSystem(const KLS_Event& e)
	{
		switch (e.System.EventType)
		{
		case KLS_SYSTEM_CLOSE: return onSystemClose(e);				break;
		case KLS_SYSTEM_WINDOWMOVED: return onSystemWindowMoved(e);		break;
		case KLS_SYSTEM_WINDOWSIZED: return onSystemWindowSized(e);		break;
		case KLS_SYSTEM_FRAMEBUFFERRESIZED: return onSystemFrameBufferResized(e); break;
		case KLS_SYSTEM_LEVEL_ADDED: return onSystemLevelAdded(e);			break;
		case KLS_SYSTEM_LEVEL_REMOVED: return onSystemLevelRemoved(e);		break;
		case KLS_SYSTEM_LEVEL_PRECHANGE: return onSystemLevelPreChange(e);		break;
		case KLS_SYSTEM_LEVEL_CHANGED: return onSystemLevelChanged(e);		break;
		case KLS_SYSTEM_WINDOWFOCUS: return onSystemWindowFocused(e);		break;
		case KLS_SYSTEM_WINDOWICONIFIED: return onSystemWindowIconified(e);	break;
		case KLS_SYSTEM_WINDOWMAXIMIZE: return onSystemWindowMaximize(e);		break;
		case KLS_SYSTEM_MOUSE_ENTER: return onSystemMouseEnter(e);			break;
		case KLS_SYSTEM_MOUSE_EXIT: return onSystemMouseExit(e);			break;
		case KLS_SYSTEM_FILENAMEDROP: return onSystemFilenameDrop(e);		break;
		}
		return KLS_FAILURE;
	}

	// parse the event into individual function calls
	bool KLS_EventHandler::onEventMouse(const KLS_Event& e)
	{
		// update the static mouse information
		m_MouseX = e.Mouse.X;
		m_MouseY = e.Mouse.Y;
		m_LButton = e.Mouse.LButton;
		m_MButton = e.Mouse.MButton;
		m_RButton = e.Mouse.RButton;

		switch (e.Mouse.EventType)
		{
		case KLS_EVT_LMOUSE_PRESSED: return onLMousePressed(e);	break;
		case KLS_EVT_MMOUSE_PRESSED: return onMMousePressed(e);	break;
		case KLS_EVT_RMOUSE_PRESSED: return onRMousePressed(e);	break;
		case KLS_EVT_LMOUSE_RELEASED: return onLMouseReleased(e);	break;
		case KLS_EVT_MMOUSE_RELEASED: return onMMouseReleased(e);	break;
		case KLS_EVT_RMOUSE_RELEASED: return onRMouseReleased(e);	break;
		case KLS_EVT_MOUSE_MOVED: return onMouseMoved(e);		break;
		case KLS_EVT_MOUSE_WHEEL: return onMouseWheel(e);		break;
		}
		return KLS_FAILURE;
	}

	// parse the event into individual function calls
	bool KLS_EventHandler::onEventKeyboard(const KLS_Event& e)
	{
		// if the key is pressed
		if (e.Keyboard.KeyDown == true)
		{
			// track keyboard states in static variable
			if (m_Keyboard[e.Keyboard.Key] != DOWN)	m_Keyboard[e.Keyboard.Key] = PRESSED;
			else m_Keyboard[e.Keyboard.Key] = DOWN;

			// parse the event to the function
			return onKeyDown(e);
		}
		else
			// otherwise the key is released
			if (m_Keyboard[e.Keyboard.Key] != UP)
			{
				// track keyboard states in static variable
				m_Keyboard[e.Keyboard.Key] = RELEASED;

				// parse the event to the function
				return onKeyUp(e);
			}

		// how we got here is beyond me.....
		return KLS_FAILURE;
	}

} // end namespace
