// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_KeyCodes.h"

// unclutter the global namespace
namespace KLS
{
	// map of keyboard keys to actions
	struct KLS_API KLS_KeyMap
	{
		std::unordered_map<KLS_ACTION, KLS_KEYCODE> m_Entries;
		KLS_KeyMap();
		~KLS_KeyMap();

		void push_back(KLS_KEYCODE key, KLS_ACTION action);	// add a mapping to the list
		KLS_KEYCODE getKey(KLS_ACTION action);				// get the action assocaited with the keyboard key
	};

	// simple prebuilt mappings
	extern KLS_KeyMap KLS_API getKeyMap_WASD();		// maybe the character keymap
	extern KLS_KeyMap KLS_API getKeyMap_Arrows();	// maybe the camera keymap

} // end namespace