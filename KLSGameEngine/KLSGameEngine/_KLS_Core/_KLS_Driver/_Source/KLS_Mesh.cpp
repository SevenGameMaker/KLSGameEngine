#include "KLS_Mesh.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

namespace KLS
{
	KLS_Mesh::KLS_Mesh(KLS_Driver* driver) : m_Driver(driver)
	{
		m_Driver = driver;
	}

	KLS_Mesh::KLS_Mesh(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData)
	{
		m_Driver = driver;
		m_VertexType = vertextype;
		addMeshBuffer(new KLS_MeshBuffer(driver, vertextype, layout, vertexData, indexData));
	}

	KLS_Mesh::~KLS_Mesh()
	{
		for (auto it : m_MeshBuffers)
		{
			delete(it);
		}
		m_MeshBuffers.clear();
		m_Driver = nullptr;
	}

	KLS_MeshBuffer* KLS_Mesh::addMeshBuffer(KLS_MeshBuffer* mb)
	{
		m_VertexType = mb->getVertexType();
		m_MeshBuffers.push_back(mb);
		mb->getMaterial().MeshBuffer = (int)m_MeshBuffers.size()-1;
		return mb;
	}

	KLS_MeshBuffer* KLS_Mesh::getMeshBuffer(int index)
	{
		if (index > m_MeshBuffers.size())
		{
			KLS_ERROR("attempting to access meshbuffer {} but it is out of range!", index);
			return nullptr;
		}

		return m_MeshBuffers[index];
	}

	// material manipulation methods
	int KLS_Mesh::getMaterialCount()
	{
		return getMeshBufferCount();
	}

	KLS_Material& KLS_Mesh::getMaterial(int index)
	{
		if (index > (int)getMeshBufferCount()) return KLS_IDENTITY_MATERIAL;
		return getMeshBuffer(index)->getMaterial();
	}

	KLS_AABBBox& KLS_Mesh::updateBoundingBox()
	{
		for (auto mb : m_MeshBuffers)
		{
			m_BoundingBox.addBox(mb->updateBoundingBox());
		}
		return m_BoundingBox;
	}

	bool KLS_Mesh::hasTransparency()
	{
		for (int x = 0; x < getMaterialCount(); x++)
		{
			if (getMaterial(x).Transparency)
			{
				return true;
			}
		}
		return false;
	}

	void KLS_Mesh::recalculateNormals()
	{
		for (auto mb : m_MeshBuffers)
			mb->calculateNormals();
	}

} // end namespace