// include the needed header files
#include "KLS_AABBBox.h"
#include "KLS_Logger.h"
#include "KLS_Line.h"
#include "KLS_BoundingSphere.h"

// unclutter the global namespace
namespace KLS
{
    // Constructor to initialize the AABB
    KLS_AABBBox::KLS_AABBBox() : m_Min(0.0f), m_Max(0.0f) {}
    KLS_AABBBox::KLS_AABBBox(glm::vec3 min, glm::vec3 max) : m_Min(min), m_Max(max) {}

    // Function to update the AABB only when necessary
    void KLS_AABBBox::updateAABB(const glm::vec3& min, const glm::vec3& max)
    {
        m_Min = min;
        m_Max = max;
    }

    // Getter functions for the AABB properties
    const glm::vec3& KLS_AABBBox::getMin() const { return m_Min; }
    const glm::vec3& KLS_AABBBox::getMax() const { return m_Max; }
    glm::vec3& KLS_AABBBox::getCenter() { m_Center = ((m_Max + m_Min) * 0.5f); return m_Center; }

    KLS_AABBBox KLS_AABBBox::Transform(const KLS_Transform& transform) const
    {
        // Define the transformation matrix using the position
        glm::mat4 transformationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(transform.m_Position));

        // Transform the minimum and maximum corners
        glm::vec3 newMin = glm::vec3(transformationMatrix * glm::vec4(m_Min, 1.0f));
        glm::vec3 newMax = glm::vec3(transformationMatrix * glm::vec4(m_Max, 1.0f));

        // Calculate the new AABB that encapsulates the transformed corners
        return KLS_AABBBox(newMin, newMax);
    }

    // Function to check if a point is inside the AABB
    bool KLS_AABBBox::contains(const glm::vec3& point) const
    {
        // Use cached AABB values for faster containment check
        return (point.x >= m_Min.x && point.x <= m_Max.x &&
                point.y >= m_Min.y && point.y <= m_Max.y &&
                point.z >= m_Min.z && point.z <= m_Max.z);
    }

    void KLS_AABBBox::addBox(const KLS_AABBBox& box)
    {
        addPoint(box.m_Min);
        addPoint(box.m_Max);
    }

    // Function to add a point to the AABB and resize it if needed
    void KLS_AABBBox::addPoint(const glm::vec3& point)
    {
       m_Min = glm::min(m_Min, point);
       m_Max = glm::max(m_Max, point);
    }

    void KLS_AABBBox::addPoints(const std::vector<float>& points)
    {
        if (points.size() % 3 != 0)
        {
            KLS_WARN("The input vector size should be a multiple of 3 for valid points (x, y, z)");
            return;
        }

        // Iterate over the input vector and add points in groups of three
        for (size_t i = 0; i < points.size(); i += 3)
        {
            glm::vec3 point(points[i], points[i + 1], points[i + 2]);
            m_Min = glm::min(m_Min, point);
            m_Max = glm::max(m_Max, point);
        }
    }

    bool KLS_AABBBox::intersects(const KLS_BoundingSphere& sphere) const
    {
        // Calculate the closest point on the AABB to the sphere's center
        glm::vec3 closestPoint = glm::clamp(sphere.m_Center, getMin(), getMax());

        // Calculate the distance squared between the sphere's center and the closest point
        float distanceSquared = glm::distance2(sphere.m_Center, closestPoint);

        // Check if the distance squared is less than or equal to the sphere's radius squared
        return distanceSquared <= (sphere.m_Radius * sphere.m_Radius);
    }

    // Function to check if another AABB intersects with this AABB
    bool KLS_AABBBox::intersects(const KLS_AABBBox& other) const
    {
        // Use cached AABB values for faster intersection check
        return !(other.m_Min.x > m_Max.x || other.m_Max.x < m_Min.x ||
            other.m_Min.y > m_Max.y || other.m_Max.y < m_Min.y ||
            other.m_Min.z > m_Max.z || other.m_Max.z < m_Min.z);
    }

    bool KLS_AABBBox::intersects(const KLS_Line& line) const
    {
        glm::vec3 dir = line.m_End - line.m_Start;

        // Calculate the inverse direction components to avoid division by zero
        glm::vec3 invDir(1.0f / dir.x, 1.0f / dir.y, 1.0f / dir.z);

        // Calculate the minimum and maximum t values for each axis
        glm::vec3 tMin = (getMin() - line.m_Start) * invDir;
        glm::vec3 tMax = (getMax() - line.m_Start) * invDir;

        // Find the largest minimum t value
        float tNear = glm::max(glm::max(glm::min(tMin.x, tMax.x), glm::min(tMin.y, tMax.y)), glm::min(tMin.z, tMax.z));

        // Find the smallest maximum t value
        float tFar = glm::min(glm::min(glm::max(tMin.x, tMax.x), glm::max(tMin.y, tMax.y)), glm::max(tMin.z, tMax.z));

        // Check if the line segment intersects the AABB
        return tNear <= tFar;
    }

} // end namespace