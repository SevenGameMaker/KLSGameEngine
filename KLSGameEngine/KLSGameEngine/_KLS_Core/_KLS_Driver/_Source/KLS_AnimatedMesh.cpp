#include "KLS_AnimatedMesh.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

namespace KLS
{
	KLS_AnimatedMesh::KLS_AnimatedMesh(KLS_Driver* driver) : KLS_Mesh(driver), KLS_Skeleton()
	{
		setSkeletonAnimatedMesh(this);
	}

	KLS_AnimatedMesh::KLS_AnimatedMesh(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData):
		KLS_Mesh(driver,vertextype,layout,vertexData,indexData)
	{
	}

	KLS_AnimatedMesh::~KLS_AnimatedMesh()
	{
	}

} // end namespace