// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_AABBBox.h"

// unclutter the global namespace
namespace KLS
{
    class KLS_API KLS_Plane
    {
    public:
        glm::vec3 Normal = {0.f, 1.f, 0.f };
        float Distance = 0.f;
        KLS_Plane() 
        {
        }
        KLS_Plane(const glm::vec3& p1, const glm::vec3& norm)
            : Normal(glm::normalize(norm)),
            Distance(glm::dot(Normal, p1))
        {}
    };

} // end namespace
