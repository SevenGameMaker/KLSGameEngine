// parse this fil eonly once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	// simple conversion methods
	extern KLS_API std::string toString(int v);
	extern KLS_API std::string toString(float v);
	extern KLS_API std::string toString(double v);
	extern KLS_API std::string keyCodetoString(int key, int scancode);

	// pad a string to a specified length
	extern KLS_API void padToLength(std::string& text, int length, char pad);

	// fill std::vectors with specified data
	extern void KLS_API fillWithFloat(std::vector<float>& dest, int count, float value);
	extern void KLS_API fillWithString(std::vector<std::string>& vec, int count, const std::string& fillString);

	// string manipulation
	extern std::vector<std::string> KLS_API splitString(const std::string& input, char delimiter);
	extern std::string KLS_API getDirectoryFromFilename(const std::string& filename);

	extern std::string KLS_API intToString(int v);
	extern std::string KLS_API floatToString(float v);
	extern std::string KLS_API boolToString(bool v);
	extern std::string KLS_API vec2ToString(glm::vec2 v);
	extern std::string KLS_API vec3ToString(glm::vec3 v);
	extern std::string KLS_API vec4ToString(glm::vec4 v);
	extern std::string KLS_API mat3ToString(glm::mat3 v);
	extern std::string KLS_API mat4ToString(glm::mat4 v);
	extern std::string KLS_API quatToString(glm::quat v);

	extern std::string KLS_API stringToString(std::string);
	extern int KLS_API stringToInt(std::string);
	extern float KLS_API stringToFloat(std::string);
	extern bool KLS_API stringToBool(std::string);
	extern glm::vec2 KLS_API stringToVec2(std::string);
	extern glm::vec3 KLS_API stringToVec3(std::string);
	extern glm::vec4 KLS_API stringToVec4(std::string);
	extern glm::mat3 KLS_API stringToMat3(std::string);
	extern glm::mat4 KLS_API stringToMat4(std::string);
	extern glm::quat KLS_API stringToQuat(std::string);

	extern std::wstring KLS_API stringToWideString(std::string text);
	extern std::string KLS_API widestringToString(std::wstring text);

} // end namepsace

