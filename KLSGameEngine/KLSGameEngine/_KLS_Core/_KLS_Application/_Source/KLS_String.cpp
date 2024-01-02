// include the needed header files
#include "KLS_String.h"
#include "KLS_KeyCodes.h"
#include "KLS_Logger.h"
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <codecvt>
#include <locale>

std::string KLS_API KLS_MATERIAL_TYPE_TEXT[] =
{
	"KLSMT_NONE", "KLSMT_UV", "KLSMT_NORMALS", "KLSMT_SKYBOX", "KLSMT_TEXTURED", "KLSMT_TERRAIN",
	"KLSMT_FSQUAD", "KLSMT_ANIMATEDMESH", "KLSMT_COLOR", "KLSMT_PARTICLES", "KLSMT_SUN",
	"KLSMT_INSTANCED", "KLSMT_BUMPMAP", "KLSMT_INFINITEPLANE",
	"KLSMT_2DGREYSCALE", "KLSMT_2DINVERTED", "KLS_MT_COUNT"
};

std::string KLS_API KLS_MODE_TEXT[] =
{
	"KLS_MODE_INIT", "KLS_MODE_RUN", "KLS_MODE_PAUSE", "KLS_MODE_RESUME", "KLS_MODE_QUIT"
};

std::string KLS_VSYNC_FLAGS_TEXT[] =
{
	"KLSVS_NONE", "KLSVS_UNLIMITED", "KLSVS_LIMITED", "KLSVS_PROTECTED"
};

std::string KLS_API KLS_PHYSXTYPE_TEXT[] =
{
	"POT_NONE", "POT_CUBE", "POT_SPHERE", "POT_TREE", "POT_COUNT"
};

// unclutter the global namespace
namespace KLS
{
	std::string KLS_API KLS_CULLMODE_STRING[] =
	{
		"KLSCM_AUTO", "KLSCM_NONE", "KLSCM_FRUSTUM", "KLSCM_DISTANCE"
	};

	std::string KLS_API KLS_PROPERTY_TYPE_STRING[] =
	{
		"KLS_PT_NONE",
		"KLS_PT_INT", "KLS_PT_FLOAT", "KLS_PT_BOOL",
		"KLS_PT_STRING",
		"KLS_PT_VEC2", "KLS_PT_VEC3", "KLS_PT_VEC4",
		"KLS_PT_MAT3", "KLS_PT_MAT4", "KLS_PT_COLOR3", "KLS_PT_COLOR4",
		"KLS_PT_MESHFILENAME", "KLS_PT_TEXTUREFILENAME","KLS_PT_TRANSFORM"
	};

	std::string getDirectoryFromFilename(const std::string& filename) {
		// Find the position of the last directory separator (usually '/').
		size_t lastSlashPos = filename.find_last_of('/');

		if (lastSlashPos != std::string::npos) {
			// Extract the directory portion of the filename and return it.
			return filename.substr(0, lastSlashPos+1);
		}
		else {
			// Return an empty string to indicate an invalid filename format.
			return "";
		}
	}

	std::vector<std::string> splitString(const std::string& input, char delimiter) {
		std::vector<std::string> tokens;
		std::stringstream ss(input);
		std::string token;

		while (std::getline(ss, token, delimiter)) {
			tokens.push_back(token);
		}

		return tokens;
	}

	void fillWithFloat(std::vector<float>& dest, int count, float value)
	{
		dest = std::vector<float>(count, value);
	}

	void fillWithString(std::vector<std::string>& vec, int count, const std::string& fillString)
	{
		vec = std::vector<std::string>(count, fillString);
	}

	std::string toString(int v)
	{
		return std::to_string(v);
	}

	std::string toString(float v)
	{
		return std::to_string(v);
	}

	std::string toString(double v)
	{
		return std::to_string(v);
	}

	void padToLength(std::string& text, int length, char pad)
	{
		if (length > text.length())
			text.append(length - text.length(), pad);
	}

	std::string keyCodetoString(int key, int scancode)
	{
		switch (key)
		{
		case KEY_UNKNOWN: return "KEY_UNKNOWN"; break;
		case KEY_SPACE: return "KEY_SPACE"; break;
		case KEY_APOSTROPHE: return "KEY_APOSTROPHE"; break;
		case KEY_COMMA: return "KEY_COMMA"; break;
		case KEY_MINUS: return "KEY_MINUS"; break;
		case KEY_PERIOD: return "KEY_PERIOD"; break;
		case KEY_SLASH: return "KEY_SLASH"; break;
		case KEY_0: return "KEY_0"; break;
		case KEY_1: return "KEY_1"; break;
		case KEY_2: return "KEY_2"; break;
		case KEY_3: return "KEY_3"; break;
		case KEY_4: return "KEY_4"; break;
		case KEY_5: return "KEY_5"; break;
		case KEY_6: return "KEY_6"; break;
		case KEY_7: return "KEY_7"; break;
		case KEY_8: return "KEY_8"; break;
		case KEY_9: return "KEY_9"; break;
		case KEY_SEMICOLON: return "KEY_SEMICOLON"; break;
		case KEY_EQUAL: return "KEY_EQUAL"; break;
		case KEY_A: return "KEY_A"; break;
		case KEY_B: return "KEY_B"; break;
		case KEY_C: return "KEY_C"; break;
		case KEY_D: return "KEY_D"; break;
		case KEY_E: return "KEY_E"; break;
		case KEY_F: return "KEY_F"; break;
		case KEY_G: return "KEY_G"; break;
		case KEY_H: return "KEY_H"; break;
		case KEY_I: return "KEY_I"; break;
		case KEY_J: return "KEY_J"; break;
		case KEY_K: return "KEY_K"; break;
		case KEY_L: return "KEY_L"; break;
		case KEY_M: return "KEY_M"; break;
		case KEY_N: return "KEY_N"; break;
		case KEY_O: return "KEY_O"; break;
		case KEY_P: return "KEY_P"; break;
		case KEY_Q: return "KEY_Q"; break;
		case KEY_R: return "KEY_R"; break;
		case KEY_S: return "KEY_S"; break;
		case KEY_T: return "KEY_T"; break;
		case KEY_U: return "KEY_U"; break;
		case KEY_V: return "KEY_V"; break;
		case KEY_W: return "KEY_W"; break;
		case KEY_X: return "KEY_X"; break;
		case KEY_Y: return "KEY_Y"; break;
		case KEY_Z: return "KEY_Z"; break;
		case KEY_LEFT_BRACKET: return "KEY_LEFT_BRACKET"; break;
		case KEY_BACKSLASH: return "KEY_BACKSLASH"; break;
		case KEY_RIGHT_BRACKET: return "KEY_RIGHT_BRACKET"; break;
		case KEY_GRAVE_ACCENT: return "KEY_GRAVE_ACCENT"; break;
		case KEY_WORLD_1: return "KEY_WORLD_1"; break;
		case KEY_WORLD_2: return "KEY_WORLD_2"; break;
		case KEY_ESCAPE: return "KEY_ESCAPE"; break;
		case KEY_ENTER: return "KEY_ENTER"; break;
		case KEY_TAB: return "KEY_TAB"; break;
		case KEY_BACKSPACE: return "KEY_BACKSPACE "; break;
		case KEY_INSERT: return "KEY_INSERT"; break;
		case KEY_DELETE: return "KEY_DELETE"; break;
		case KEY_RIGHT: return "KEY_RIGHT"; break;
		case KEY_LEFT: return "KEY_LEFT"; break;
		case KEY_DOWN: return "KEY_DOWN"; break;
		case KEY_UP: return "KEY_UP"; break;
		case KEY_PAGE_UP: return "KEY_PAGE_UP"; break;
		case KEY_PAGE_DOWN: return "KEY_PAGE_DOWN"; break;
		case KEY_HOME: return "KEY_HOME"; break;
		case KEY_END: return "KEY_END"; break;
		case KEY_CAPS_LOCK: return "KEY_CAPS_LOCK"; break;
		case KEY_SCROLL_LOCK: return "KEY_SCROLL_LOCK"; break;
		case KEY_NUM_LOCK: return "KEY_NUM_LOCK"; break;
		case KEY_PRINT_SCREEN: return "KEY_PRINT_SCREEN"; break;
		case KEY_PAUSE: return "KEY_PAUSE"; break;
		case KEY_F1: return "KEY_F1"; break;
		case KEY_F2: return "KEY_F2"; break;
		case KEY_F3: return "KEY_F3"; break;
		case KEY_F4: return "KEY_F4"; break;
		case KEY_F5: return "KEY_F5"; break;
		case KEY_F6: return "KEY_F6"; break;
		case KEY_F7: return "KEY_F7"; break;
		case KEY_F8: return "KEY_F8"; break;
		case KEY_F9: return "KEY_F9"; break;
		case KEY_F10: return "KEY_F10"; break;
		case KEY_F11: return "KEY_F11"; break;
		case KEY_F12: return "KEY_F12"; break;
		case KEY_F13: return "KEY_F13"; break;
		case KEY_F14: return "KEY_F14"; break;
		case KEY_F15: return "KEY_F15"; break;
		case KEY_F16: return "KEY_F16"; break;
		case KEY_F17: return "KEY_F17"; break;
		case KEY_F18: return "KEY_F18"; break;
		case KEY_F19: return "KEY_F19"; break;
		case KEY_F20: return "KEY_F20"; break;
		case KEY_F21: return "KEY_F21"; break;
		case KEY_F22: return "KEY_F22"; break;
		case KEY_F23: return "KEY_F23"; break;
		case KEY_F24: return "KEY_F24"; break;
		case KEY_F25: return "KEY_F25"; break;
		case KEY_KP_0: return "KEY_KP_0"; break;
		case KEY_KP_1: return "KEY_KP_1"; break;
		case KEY_KP_2: return "KEY_KP_2"; break;
		case KEY_KP_3: return "KEY_KP_3"; break;
		case KEY_KP_4: return "KEY_KP_4"; break;
		case KEY_KP_5: return "KEY_KP_5"; break;
		case KEY_KP_6: return "KEY_KP_6"; break;
		case KEY_KP_7: return "KEY_KP_7"; break;
		case KEY_KP_8: return "KEY_KP_8"; break;
		case KEY_KP_9: return "KEY_KP_9"; break;
		case KEY_KP_DECIMAL: return "KEY_KP_DECIMAL"; break;
		case KEY_KP_DIVIDE: return "KEY_KP_DIVIDE"; break;
		case KEY_KP_MULTIPLY: return "KEY_KP_MULTIPLY"; break;
		case KEY_KP_SUBTRACT: return "KEY_KP_SUBTRACT"; break;
		case KEY_KP_ADD: return "KEY_KP_ADD"; break;
		case KEY_KP_ENTER: return "KEY_KP_ENTER"; break;
		case KEY_KP_EQUAL: return "KEY_KP_EQUAL"; break;
		case KEY_LEFT_SHIFT: return "KEY_LEFT_SHIFT"; break;
		case KEY_LEFT_CONTROL: return "KEY_LEFT_CONTROL"; break;
		case KEY_LEFT_ALT: return "KEY_LEFT_ALT"; break;
		case KEY_LEFT_SUPER: return "KEY_LEFT_SUPER"; break;
		case KEY_RIGHT_SHIFT: return "KEY_RIGHT_SHIFT"; break;
		case KEY_RIGHT_CONTROL: return "KEY_RIGHT_CONTROL"; break;
		case KEY_RIGHT_ALT: return "KEY_RIGHT_ALT"; break;
		case KEY_RIGHT_SUPER: return "KEY_RIGHT_SUPER"; break;
		case KEY_MENU: return "KEY_MENU"; break;
		}
		return KLS_DEFAULT_STRING;
	}
	std::string KLS_EVENTTYPE_STRING[] = {
		"KLS_EVT_SYSTEM",
		"KLS_EVT_KEYBOARD",
		"KLS_EVT_MOUSE",
		"KLS_EVT_COUNT" };

	std::string KLS_EVENT_SYSTEM_STRING[] = 
	{
		"KLS_SYSTEM_CLOSE",
		"KLS_SYSTEM_WINDOWMOVED",
		"KLS_SYSTEM_WINDOWSIZED",
		"KLS_SYSTEM_WINDOWFOCUS",
		"KLS_SYSTEM_WINDOWICONIFIED",
		"KLS_SYSTEM_WINDOWMAXIMIZE",
		"KLS_SYSTEM_FRAMEBUFFERRESIZED",
		"KLS_SYSTEM_MOUSE_ENTER",
		"KLS_SYSTEM_MOUSE_EXIT",
		"KLS_SYSTEM_LEVEL_ADDED",
		"KLS_SYSTEM_LEVEL_REMOVED",
		"KLS_SYSTEM_LEVEL_PRECHANGE",
		"KLS_SYSTEM_LEVEL_CHANGED",
		"KLS_SYSTEM_FILENAMEDROP",
		"KLS_SYSTEM_COUNT"
	};

	std::string KLS_EVENT_MOUSE_STRING[] = {
		"KLS_EVT_LMOUSE_PRESSED",
		"KLS_EVT_MMOUSE_PRESSED",
		"KLS_EVT_RMOUSE_PRESSED",
		"KLS_EVT_LMOUSE_RELEASED",
		"KLS_EVT_MMOUSE_RELEASED",
		"KLS_EVT_RMOUSE_RELEASED",
		"KLS_EVT_MOUSE_MOVED",
		"KLS_EVT_MOUSE_WHEEL",
		"KLS_EVT_MOUSE_COUNT"
	};

	std::string intToString(int v) 
	{
		return std::to_string(v);
	}

	std::string floatToString(float v) 
	{
		return std::to_string(v);
	}

	std::string boolToString(bool v) 
	{
		return std::to_string(v);
	}

	std::string vec2ToString(glm::vec2 v) 
	{
		std::ostringstream oss;
		oss << v.x << ' ' << v.y;
		return oss.str();
	}

	std::string vec3ToString(glm::vec3 v) 
	{
		std::ostringstream oss;
		oss << v.x << ' ' << v.y << ' ' << v.z;
		return oss.str();
	}

	std::string vec4ToString(glm::vec4 v) 
	{
		std::ostringstream oss;
		oss << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
		return oss.str();
	}

	std::string mat3ToString(glm::mat3 v) 
	{
		std::string result;
		for (int i = 0; i < 3; ++i) 
		{
			for (int j = 0; j < 3; ++j) 
			{
				result += floatToString(v[i][j]);
				if (i != 2 || j != 2) 
				{
					result += " ";
				}
			}
		}
		return result;
	}

	std::string mat4ToString(glm::mat4 v) 
	{
		std::string result;
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				result += floatToString(v[i][j]);
				if (i != 3 || j != 3) {
					result += " ";
				}
			}
		}
		return result;
	}

	std::string quatToString(glm::quat v)
	{
		std::ostringstream oss;
		oss << v.x << ' ' << v.y << ' ' << v.z << ' ' << v.w;
		return oss.str();
	}

	std::string stringToString(std::string str) 
	{
		return str;
	}

	int stringToInt(std::string str) 
	{
		return std::stoi(str);
	}

	float stringToFloat(std::string str) 
	{
		return std::stof(str);
	}

	bool stringToBool(std::string str) 
	{
		return std::stoi(str);
	}

	glm::vec2 stringToVec2(std::string str)
	{
		glm::vec4 vec;
		std::istringstream iss(str);
		iss >> vec.x >> vec.y;
		return vec;
	}

	glm::vec3 stringToVec3(std::string str)
	{
		glm::vec3 vec;
		std::istringstream iss(str);
		iss >> vec.x >> vec.y >> vec.z;
		return vec;
	}

	glm::vec4 stringToVec4(std::string str) 
	{
		glm::vec4 vec;
		std::istringstream iss(str);
		iss >> vec.x >> vec.y >> vec.z >> vec.w;
		return vec;
	}

	glm::mat3 stringToMat3(std::string str) {
		glm::mat3 result = glm::mat3(0);
		if (!str.empty()) {
			std::istringstream ss(str);
			for (int i = 0; i < 9; ++i) {
				if (!(ss >> result[i / 3][i % 3])) {
					return glm::mat3(0);
				}
			}
		}
		return result;
	}

	glm::mat4 stringToMat4(std::string str) {
		glm::mat4 result = glm::mat4(0);
		if (!str.empty()) {
			std::istringstream ss(str);
			for (int i = 0; i < 16; ++i) {
				if (!(ss >> result[i / 4][i % 4])) {
					return glm::mat4(0);
				}
			}
		}
		return result;
	}

	glm::quat KLS_API stringToQuat(std::string str) {
		glm::quat result = KLS_IDENTITY_QUAT;
		if (!str.empty()) {
			std::istringstream ss(str);
			float x, y, z, w;
			if (ss >> x >> y >> z >> w) {
				result = glm::quat(w, x, y, z);
			}
		}
		return result;
	}


	std::wstring stringToWideString(std::string text) 
	{
		int size = MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, nullptr, 0);
		std::wstring result(size, 0);
		MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, &result[0], size);
		return result;
	}

	std::string widestringToString(std::wstring text)
	{
		int size = WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::string result(size, 0);
		WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, &result[0], size, nullptr, nullptr);
		return result;
	}

} // end namespace