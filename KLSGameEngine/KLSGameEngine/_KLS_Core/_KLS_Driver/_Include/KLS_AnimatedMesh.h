// parse this file only once
#pragma once

// include the needed header files
#include "KLS_Mesh.h"
#include "KLS_Bone.h"
#include "KLS_Animator.h"
#include "KLS_Bone.h"
#include "KLS_Skeleton.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_Skeleton KLS_Skeleton;

	// simple mesh class that holds multiple meshbuffers
	class KLS_API KLS_AnimatedMesh : public KLS_Mesh, public KLS_Skeleton
	{
	public:
		// default constructor
		KLS_AnimatedMesh(KLS_Driver* driver);

		// creates a fully functional mesh object with one meshbuffer
		KLS_AnimatedMesh(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData);

		// class destructor
		~KLS_AnimatedMesh();
	};

} // end namespace