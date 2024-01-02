// include the needed header files
#include "KLS_BoundingSphere.h"
#include "KLS_Line.h"
#include "KLS_AABBBox.h"

// unclutter the global namespace
namespace KLS
{
    // Constructor implementation
    KLS_BoundingSphere::KLS_BoundingSphere(const glm::vec3& center, float radius)
        : m_Center(center), m_Radius(radius) {}

    // Function to check if this sphere intersects another sphere
    bool KLS_BoundingSphere::intersects(const KLS_BoundingSphere& other) const
    {
        float distanceSquared = glm::distance2(m_Center, other.m_Center);
        float radiiSumSquared = (m_Radius + other.m_Radius) * (m_Radius + other.m_Radius);

        return distanceSquared <= radiiSumSquared;
    }

    // Function to check if this sphere intersects an AABBBox
    bool KLS_BoundingSphere::intersects(const KLS_AABBBox& box) const
    {
        // Calculate the closest point on the AABB to the sphere's center
        glm::vec3 closestPoint = glm::clamp(m_Center, box.getMin(), box.getMax());

        // Calculate the distance squared between the sphere's center and the closest point
        float distanceSquared = glm::distance2(m_Center, closestPoint);

        // Check if the distance squared is less than or equal to the sphere's radius squared
        return distanceSquared <= (m_Radius * m_Radius);
    }

    bool KLS_BoundingSphere::intersects(const KLS_Line& line) const
    {
        glm::vec3 lineDir = glm::normalize(line.m_End - line.m_Start);

        // Vector from the start of the line to the sphere's center
        glm::vec3 lineToSphere = m_Center - line.m_Start;

        // Calculate the projection of lineToSphere onto lineDir
        float projection = glm::dot(lineToSphere, lineDir);

        // Calculate the closest point on the line to the sphere's center
        glm::vec3 closestPoint;

        if (projection <= 0.0f)
        {
            // Closest point is the line's start
            closestPoint = line.m_Start;
        }
        else if (projection >= glm::length(lineDir))
        {
            // Closest point is the line's end
            closestPoint = line.m_End;
        }
        else
        {
            // Closest point is along the line
            closestPoint = line.m_Start + projection * lineDir;
        }

        // Calculate the distance between the closest point and the sphere's center
        float distance = glm::distance(m_Center, closestPoint);

        // Check if the distance is less than or equal to the sphere's radius
        return distance <= m_Radius;
    }
} // end namespace
