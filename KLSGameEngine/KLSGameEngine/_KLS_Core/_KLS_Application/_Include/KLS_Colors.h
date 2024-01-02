// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	// a few simple color defines 
#define KLS_BLACK	glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)	// glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
#define KLS_WHITE	glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)	// glm::vec4(1.0f, 1.0f, 1.0f, 1.0f)
#define KLS_RED		glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)	// glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)
#define KLS_GREEN	glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)	// glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)
#define KLS_BLUE	glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)	// glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define KLS_YELLOW	glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)	// glm::vec4(1.0f, 1.0f, 0.0f, 1.0f)
#define KLS_CYAN	glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)	// glm::vec4(0.0f, 1.0f, 1.0f, 1.0f)
#define KLS_MAGENTA glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)	// glm::vec4(1.0f, 0.0f, 1.0f, 1.0f)

#define KLS_LIGHTWHITE	glm::vec4(0.4f, 0.4f, 0.4f, 1.0f)	// glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define KLS_LIGHTBLUE	glm::vec4(0.0f, 0.0f, 0.4f, 1.0f)	// glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)
#define KLS_LIGHTRED	glm::vec4(.4f, 0.0f, 0.0f, 1.0f)	// glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)

#define KLS_HALF_TRANSPARENT glm::vec4(1.0f,1.0f,1.0f,0.5f)

} // end namespace

