// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Transform.h"

// unclutter the global namespace
namespace KLS
{
    typedef class KLS_Line KLS_Line;
    typedef class KLS_BoundingSphere KLS_BoundingSphere;
    
    #define KLS_IDENTITY_AABBBOX KLS_AABBBox(glm::vec3(-.5f), glm::vec3(.5f))

    class KLS_API KLS_AABBBox
    {
    public:
        glm::vec3 m_Min;  // Minimum corner of the AABB
        glm::vec3 m_Max;  // Maximum corner of the AABB
        glm::vec3 m_Center; // center of the box

        // Constructor to initialize the AABB
        KLS_AABBBox();
        KLS_AABBBox(glm::vec3 min, glm::vec3 max);

        // Function to update the AABB only when necessary
        void updateAABB(const glm::vec3& min, const glm::vec3& max);

        // Getter functions for the AABB properties
        const glm::vec3& getMin() const;
        const glm::vec3& getMax() const;
        glm::vec3& getCenter();

        // perform the transformation
        KLS_AABBBox Transform(const KLS_Transform& transform) const;

        // Function to check if a point is inside the AABB
        bool contains(const glm::vec3& point) const;

        // Function to check if this shape interesects with another shape
        bool intersects(const KLS_BoundingSphere& other) const;
        bool intersects(const KLS_AABBBox& other) const;
        bool intersects(const KLS_Line& other) const;

        // Function to add a point to the AABB and resize it if needed
        void addPoint(const glm::vec3& point);
        void addPoints(const std::vector<float>& points);

        // add the new box extents to this one
        void addBox(const KLS_AABBBox& box);
    
    };

}  // end namespace
