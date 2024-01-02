// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// include the specific header files
#include "KLS_String.h"

// unclutter the global namespace
namespace KLS
{
	/*
	* event system
	* an event is a union so only one field has valid data
	* this is to conserve memory and footprint
	*/

	// extern string holders. if you add an event, make sure to update the strings in KLS_String.cpp also!
	// these are used during logging of an event to identify the event to the user
	extern KLS_API std::string KLS_EVENTTYPE_STRING[];
	extern KLS_API std::string KLS_EVENT_SYSTEM_STRING[];
	extern KLS_API std::string KLS_EVENT_MOUSE_STRING[];

	// keyboard states
	enum keyStatesENUM { UP, DOWN, PRESSED, RELEASED };

	// top level event types
	enum KLS_API KLS_EVENTTYPE
	{
		KLS_EVT_SYSTEM,
		KLS_EVT_KEYBOARD,
		KLS_EVT_MOUSE,
		KLS_EVT_COUNT
	};

	// system secondary event types
	enum KLS_API KLS_SYSTEM_EVENT
	{
		KLS_SYSTEM_CLOSE,
		KLS_SYSTEM_WINDOWMOVED,
		KLS_SYSTEM_WINDOWSIZED,
		KLS_SYSTEM_WINDOWFOCUS,
		KLS_SYSTEM_WINDOWICONIFIED,
		KLS_SYSTEM_WINDOWMAXIMIZE,
		KLS_SYSTEM_FRAMEBUFFERRESIZED,
		KLS_SYSTEM_MOUSE_ENTER,
		KLS_SYSTEM_MOUSE_EXIT,
		KLS_SYSTEM_LEVEL_ADDED,
		KLS_SYSTEM_LEVEL_REMOVED,
		KLS_SYSTEM_LEVEL_PRECHANGE,
		KLS_SYSTEM_LEVEL_CHANGED,
		KLS_SYSTEM_FILENAMEDROP,
		KLS_SYSTEM_SCENENODE_ADDED,
		KLS_SYSTEM_SCENENODE_DESTROYED,
		KLS_SYSTEM_COUNT
	};

	// mouse input secondary event types
	enum KLS_API KLS_MOUSE_INPUT_EVENT
	{
		KLS_EVT_LMOUSE_PRESSED,
		KLS_EVT_MMOUSE_PRESSED,
		KLS_EVT_RMOUSE_PRESSED,
		KLS_EVT_LMOUSE_RELEASED,
		KLS_EVT_MMOUSE_RELEASED,
		KLS_EVT_RMOUSE_RELEASED,
		KLS_EVT_MOUSE_MOVED,
		KLS_EVT_MOUSE_WHEEL,
		KLS_EVT_MOUSE_COUNT
	};

	// an event.
	// EventType holds the top level event type
	// one of the event structures holds the pertinent data
	struct KLS_API KLS_Event
	{
		// top level event type
		KLS_EVENTTYPE	EventType;
		int				FromID = 0;
		std::string		FromName = "default";

		//! Any kind of system event.
		struct KLS_Event_System
		{
			KLS_SYSTEM_EVENT	EventType;	// secondary event type
			WPARAM				wParam;		// param
			LPARAM				lParam;		// param
			int					data0;		// user data
			int					data1;		// user data
		};

		//! Any kind of keyboard event.
		struct KLS_Event_Keyboard
		{
			bool	KeyDown;	// key id down
			int		Key;		// key that is active
			int		Scancode;	// scanecode of active key
		};

		//! Any kind of mouse event.
		struct KLS_Event_Mouse
		{
			KLS_MOUSE_INPUT_EVENT	EventType;	// secondary event type
			int						X;			// mouse X position
			int						Y;			// mouse Y position
			int						Wheel;		// wheel value
			bool					Shift;		// is shift key pressed
			bool					Control;	// is control key pressed
			bool					LButton;	// is left button pressed
			bool					MButton;	// is middle button pressed
			bool					RButton;	// is right button presses
		};

		// the event structures. only one has valid data based on the EventType
		KLS_Event_System	System;
		KLS_Event_Keyboard	Keyboard;
		KLS_Event_Mouse		Mouse;
	};

} // end namespace

