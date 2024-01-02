// parse this file only once
#pragma once

// include the required header file
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

namespace KLS
{
    typedef class KLS_Line KLS_Line;
    typedef class KLS_AABBBox KLS_AABBBox;

    class KLS_API KLS_BoundingSphere
    {
    public:
        glm::vec3 m_Center;  // Center of the sphere
        float m_Radius;     // Radius of the sphere

        // Constructor to initialize the sphere
        KLS_BoundingSphere(const glm::vec3& center, float radius);

        // Function to check if this shape interesects with another shape
        bool intersects(const KLS_BoundingSphere& other) const;
        bool intersects(const KLS_AABBBox& other) const;
        bool intersects(const KLS_Line& other) const;
    };
} // end namespace
