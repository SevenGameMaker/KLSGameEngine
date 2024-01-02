// parse this file only once
#pragma once

// include the required header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Plane.h"
#include "KLS_AABBBox.h"

// unclutter the global namespace
namespace KLS
{
	class KLS_Frustum
	{
	public:
		KLS_Frustum() {}

		// m = ProjectionMatrix * ViewMatrix 
		KLS_Frustum(glm::mat4 m);

		// http://iquilezles.org/www/articles/frustumcorrect/frustumcorrect.htm
		bool isBoxInside(const KLS_AABBBox& box) const;

		enum Planes
		{
			Left = 0,
			Right,
			Bottom,
			Top,
			Near,
			Far,
			Count,
			Combinations = Count * (Count - 1) / 2
		};

		template<Planes i, Planes j>
		struct ij2k
		{
			enum { k = i * (9 - i) / 2 + j - 1 };
		};

		template<Planes a, Planes b, Planes c>
		glm::vec3 intersection(const glm::vec3* crosses) const;

		glm::vec4   m_planes[Count];
		glm::vec3   m_points[8];
	};

	template<KLS_Frustum::Planes a, KLS_Frustum::Planes b, KLS_Frustum::Planes c>
	inline glm::vec3 KLS_Frustum::intersection(const glm::vec3* crosses) const
	{
		float D = glm::dot(glm::vec3(m_planes[a]), crosses[ij2k<b, c>::k]);
		glm::vec3 res = glm::mat3(crosses[ij2k<b, c>::k], -crosses[ij2k<a, c>::k], crosses[ij2k<a, b>::k]) *
			glm::vec3(m_planes[a].w, m_planes[b].w, m_planes[c].w);
		return res * (-1.0f / D);
	}

} // end namespace