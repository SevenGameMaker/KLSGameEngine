#include "KLS_KeyMap.h"

// unclutter the global namespace
namespace KLS
{
	KLS_KeyMap::KLS_KeyMap() {}
	KLS_KeyMap::~KLS_KeyMap() {}

	void KLS_KeyMap::push_back(KLS_KEYCODE key, KLS_ACTION action)
	{
		m_Entries.emplace(std::make_pair(action, key));
	}

	KLS_KEYCODE KLS_KeyMap::getKey(KLS_ACTION action)
	{
		if (m_Entries.find(action) != m_Entries.end()) return m_Entries[action];
		return KLS_KEYCODE::KEY_UNKNOWN;
	}

	KLS_KeyMap getKeyMap_WASD()
	{
		KLS_KeyMap keymap;
		keymap.push_back(KLS_KEYCODE::KEY_W, KLS_MOVE_FORWARD);
		keymap.push_back(KLS_KEYCODE::KEY_S, KLS_MOVE_BACKWARD);
		keymap.push_back(KLS_KEYCODE::KEY_A, KLS_MOVE_TURNLEFT);
		keymap.push_back(KLS_KEYCODE::KEY_D, KLS_MOVE_TURNRIGHT);
		keymap.push_back(KLS_KEYCODE::KEY_Q, KLS_MOVE_STRAFELEFT);
		keymap.push_back(KLS_KEYCODE::KEY_E, KLS_MOVE_STRAFERIGHT);
		keymap.push_back(KLS_KEYCODE::KEY_SPACE, KLS_MOVE_JUMP);
		keymap.push_back(KLS_KEYCODE::KEY_LEFT_ALT, KLS_MOVE_CROUCH);
		keymap.push_back(KLS_KEYCODE::KEY_LEFT_SHIFT, KLS_MOVE_RUN);
		return keymap;
	}

	KLS_KeyMap getKeyMap_Arrows()
	{
		KLS_KeyMap keymap;
		keymap.push_back(KLS_KEYCODE::KEY_UP, KLS_MOVE_FORWARD);
		keymap.push_back(KLS_KEYCODE::KEY_DOWN, KLS_MOVE_BACKWARD);
		keymap.push_back(KLS_KEYCODE::KEY_LEFT, KLS_MOVE_TURNLEFT);
		keymap.push_back(KLS_KEYCODE::KEY_RIGHT, KLS_MOVE_TURNRIGHT);
		keymap.push_back(KLS_KEYCODE::KEY_Q, KLS_MOVE_STRAFELEFT);
		keymap.push_back(KLS_KEYCODE::KEY_E, KLS_MOVE_STRAFERIGHT);
		keymap.push_back(KLS_KEYCODE::KEY_SPACE, KLS_MOVE_JUMP);
		keymap.push_back(KLS_KEYCODE::KEY_LEFT_ALT, KLS_MOVE_CROUCH);
		keymap.push_back(KLS_KEYCODE::KEY_LEFT_SHIFT, KLS_MOVE_RUN);
		return keymap;
	}

} // end namespace