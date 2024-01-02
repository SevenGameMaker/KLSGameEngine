// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
    // calculate a random integer
    int KLS_API getRandomInt(int min, int max);

    // calculate a random float
    float KLS_API getRandomFloat(float min, float max);

    // calculate a random color
    glm::vec4 KLS_API getRandomColor(float alpha = 1.0f);

    bool KLS_API decomposeTransform(const glm::mat4& transform, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);

    bool KLS_API isPointInsideRect(const glm::vec4& rect, const glm::vec2& point);

} // end namespace
