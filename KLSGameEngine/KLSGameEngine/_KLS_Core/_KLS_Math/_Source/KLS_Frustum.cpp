// include the needed header files
#include "KLS_Frustum.h"

// unclutter the global namespace
namespace KLS
{
	KLS_Frustum::KLS_Frustum(glm::mat4 m)
	{
		m = glm::transpose(m);
		m_planes[Left] = m[3] + m[0];
		m_planes[Right] = m[3] - m[0];
		m_planes[Bottom] = m[3] + m[1];
		m_planes[Top] = m[3] - m[1];
		m_planes[Near] = m[3] + m[2];
		m_planes[Far] = m[3] - m[2];

		glm::vec3 crosses[Combinations] = {
			glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Right])),
			glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Bottom])),
			glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Top])),
			glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Near])),
			glm::cross(glm::vec3(m_planes[Left]),   glm::vec3(m_planes[Far])),
			glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Bottom])),
			glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Top])),
			glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Near])),
			glm::cross(glm::vec3(m_planes[Right]),  glm::vec3(m_planes[Far])),
			glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Top])),
			glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Near])),
			glm::cross(glm::vec3(m_planes[Bottom]), glm::vec3(m_planes[Far])),
			glm::cross(glm::vec3(m_planes[Top]),    glm::vec3(m_planes[Near])),
			glm::cross(glm::vec3(m_planes[Top]),    glm::vec3(m_planes[Far])),
			glm::cross(glm::vec3(m_planes[Near]),   glm::vec3(m_planes[Far]))
		};

		m_points[0] = intersection<Left, Bottom, Near>(crosses);
		m_points[1] = intersection<Left, Top, Near>(crosses);
		m_points[2] = intersection<Right, Bottom, Near>(crosses);
		m_points[3] = intersection<Right, Top, Near>(crosses);
		m_points[4] = intersection<Left, Bottom, Far>(crosses);
		m_points[5] = intersection<Left, Top, Far>(crosses);
		m_points[6] = intersection<Right, Bottom, Far>(crosses);
		m_points[7] = intersection<Right, Top, Far>(crosses);

	}

#if(0)
	bool KLS_Frustum::isBoxInside(const KLS_AABBBox& box) const
	{
		glm::vec3 corners[8];
		corners[0] = box.m_Min;
		corners[1] = glm::vec3(box.m_Max.x, box.m_Min.y, box.m_Min.z);
		corners[2] = glm::vec3(box.m_Min.x, box.m_Max.y, box.m_Min.z);
		corners[3] = glm::vec3(box.m_Max.x, box.m_Max.y, box.m_Min.z);
		corners[4] = glm::vec3(box.m_Min.x, box.m_Min.y, box.m_Max.z);
		corners[5] = glm::vec3(box.m_Max.x, box.m_Min.y, box.m_Max.z);
		corners[6] = glm::vec3(box.m_Min.x, box.m_Max.y, box.m_Max.z);
		corners[7] = box.m_Max;

		for (int i = 0; i < Count; i++)
		{
			bool allOutside = true;
			for (int j = 0; j < 8; j++)
			{
				if (glm::dot(m_planes[i], glm::vec4(corners[j].x, corners[j].y, corners[j].z, 1.0f)) >= 0.0)
				{
					allOutside = false;
					break; // No need to check this plane further
				}
			}
			if (allOutside)
			{
				return false;
			}
		}

		return true;
	}
#else
	bool KLS_Frustum::isBoxInside(const KLS_AABBBox& box) const
	{
		glm::vec3 minp = box.m_Min;
		glm::vec3 maxp = box.m_Max;

		// check box outside/inside of frustum
		for (int i = 0; i < Count; i++)
		{
			if ((glm::dot(m_planes[i], glm::vec4(minp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(maxp.x, minp.y, minp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(minp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(maxp.x, maxp.y, minp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(minp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(maxp.x, minp.y, maxp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(minp.x, maxp.y, maxp.z, 1.0f)) < 0.0) &&
				(glm::dot(m_planes[i], glm::vec4(maxp.x, maxp.y, maxp.z, 1.0f)) < 0.0))
			{
				return false;
			}
		}

		// check frustum outside/inside box
		int out;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].x > maxp.x) ? 1 : 0); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].x < minp.x) ? 1 : 0); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].y > maxp.y) ? 1 : 0); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].y < minp.y) ? 1 : 0); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].z > maxp.z) ? 1 : 0); if (out == 8) return false;
		out = 0; for (int i = 0; i < 8; i++) out += ((m_points[i].z < minp.z) ? 1 : 0); if (out == 8) return false;

		return true;
	}
#endif
} // end namespace
