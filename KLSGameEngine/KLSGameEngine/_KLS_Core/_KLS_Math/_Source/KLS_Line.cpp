// include the needed header files
#include "KLS_Line.h"
#include "KLS_AABBBox.h"
#include "KLS_BoundingSphere.h"

// unclutter the global namespace
namespace KLS
{
    // Constructor implementation
    KLS_Line::KLS_Line(const glm::vec3& start, const glm::vec3& end)
        : m_Start(start), m_End(end) {}

    // Function to check if this intersects another shape
    bool KLS_Line::intersects(const KLS_BoundingSphere& sphere) const
    {
        // Calculate the direction vector of the line
        glm::vec3 lineDir = glm::normalize(m_End - m_Start);

        // Vector from the start of the line to the sphere's center
        glm::vec3 lineToSphere = sphere.m_Center - m_Start;

        // Calculate the projection of lineToSphere onto lineDir
        float projection = glm::dot(lineToSphere, lineDir);

        // Calculate the closest point on the line to the sphere's center
        glm::vec3 closestPoint;

        if (projection <= 0.0f)
        {
            // Closest point is the line's start
            closestPoint = m_Start;
        }
        else if (projection >= glm::length(lineDir))
        {
            // Closest point is the line's end
            closestPoint = m_End;
        }
        else
        {
            // Closest point is along the line
            closestPoint = m_Start + projection * lineDir;
        }

        // Calculate the distance between the closest point and the sphere's center
        float distance = glm::distance(sphere.m_Center, closestPoint);

        // Check if the distance is less than or equal to the sphere's radius
        return distance <= sphere.m_Radius;
    }

    // Function to check if this intersects another shape
    bool KLS_Line::intersects(const KLS_AABBBox& box) const
    {
        // Check for intersection with each AABB face
        for (int i = 0; i < 3; ++i)
        {
            glm::vec3 normal(0.0f);
            normal[i] = 1.0f;

            float denom = glm::dot(normal, m_End - m_Start);

            if (denom == 0.0f)
            {
                // The line is parallel to this face; skip it
                continue;
            }

            float t = glm::dot(box.getMin() - m_Start, normal) / denom;

            if (t >= 0.0f && t <= 1.0f)
            {
                // Calculate the intersection point
                glm::vec3 intersection = m_Start + t * (m_End - m_Start);

                // Check if the intersection point is inside the AABB
                if (box.contains(intersection))
                {
                    return true;
                }
            }
        }

        return false;
    }

    // Function to check if this intersects another shape
    bool KLS_Line::intersects(const KLS_Line& other) const
    {
        glm::vec3 dir1 = m_End - m_Start;
        glm::vec3 dir2 = other.m_End - other.m_Start;
        glm::vec3 diff = m_Start - other.m_Start;

        // Calculate the cross products
        glm::vec3 cross1 = glm::cross(diff, dir1);
        glm::vec3 cross2 = glm::cross(dir2, dir1);

        float t = glm::dot(cross1, cross2) / glm::length2(cross2);

        // Check if 't' is within the valid range for both lines
        return (t >= 0.0f && t <= 1.0f);
    }



} // end namespace
