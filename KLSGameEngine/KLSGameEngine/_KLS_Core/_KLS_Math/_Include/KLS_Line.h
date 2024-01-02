// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

namespace KLS
{
    typedef class KLS_AABBBox KLS_AABBBox;
    typedef class KLS_BoundingSphere KLS_BoundingSphere;

    class KLS_API KLS_Line
    {
    public:
        glm::vec3 m_Start;  // Starting point of the line
        glm::vec3 m_End;    // Ending point of the line

        // Constructor to initialize the line
        KLS_Line(const glm::vec3& start, const glm::vec3& end);

        // Function to check if this shape interesects with another shape
        bool intersects(const KLS_BoundingSphere& other) const;
        bool intersects(const KLS_AABBBox& other) const;
        bool intersects(const KLS_Line& other) const;
    };

} // end namespace


