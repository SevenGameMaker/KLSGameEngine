#include "KLS_MeshBuffer.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

namespace KLS
{
	KLS_MeshBuffer::KLS_MeshBuffer(KLS_Driver* driver)
		: m_Driver(driver)
	{
		m_VAO = driver->instantiateVertexArray();
	}

	KLS_MeshBuffer::KLS_MeshBuffer(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData)
		: m_Driver(driver)
	{
		m_VertexType = vertextype;

		KLS_VertexBuffer* vbo = driver->instantiateVertexBuffer();
		KLS_IndexBuffer* ibo = driver->instantiateIndexBuffer();
		m_VAO = driver->instantiateVertexArray();
		m_VAO->bind();

		vbo->setData(vertextype, layout, vertexData);
		ibo->setData(indexData);

		m_VAO->addVertexBuffer(vbo);
		m_VAO->setIndexBuffer(ibo);
	}

	KLS_MeshBuffer::~KLS_MeshBuffer()
	{
		if (m_VAO) delete(m_VAO); m_VAO = nullptr;
		m_Driver = nullptr;
	}

	void KLS_MeshBuffer::bind()
	{
		if (m_VAO) m_VAO->bind();
	}

	void KLS_MeshBuffer::unbind()
	{
		if (m_VAO) m_VAO->unbind();
	}

	void KLS_MeshBuffer::addVertexBuffer(KLS_VertexBuffer* vertexBuffer)
	{
		if (m_VAO) m_VAO->addVertexBuffer(vertexBuffer);
	}

	void KLS_MeshBuffer::setIndexBuffer(KLS_IndexBuffer* indexBuffer)
	{
		if (m_VAO) m_VAO->setIndexBuffer(indexBuffer);
	}

	void KLS_MeshBuffer::calculateNormals()
	{
		return;
		// make sure the vao is valid
		if (m_VAO)
		{
			for (int i = 0; i < m_VAO->getVertexBufferCount(); ++i)
			{
				KLS_VertexBuffer* vbo = m_VAO->getVertexBuffer(i);

				for (int i = 0; i < m_VAO->getIndexBuffer()->getCount(); i += 3)
				{
					int size = getVertexSize(vbo->getVertexType());
					glm::vec3 p1, p2, p3, n1, n2, n3;
					p1.x = vbo->m_Data[i * size];
					p1.y = vbo->m_Data[i * size + 1];
					p1.z = vbo->m_Data[i * size + 2];
					n1.x = vbo->m_Data[i * size + 6];
					n1.y = vbo->m_Data[i * size + 7];
					n1.z = vbo->m_Data[i * size + 8];

					p2.x = vbo->m_Data[(i + 1) * size];
					p2.y = vbo->m_Data[(i + 1) * size + 1];
					p2.z = vbo->m_Data[(i + 1) * size + 2];
					n2.x = vbo->m_Data[(i + 1) * size + 6];
					n2.y = vbo->m_Data[(i + 1) * size + 7];
					n2.z = vbo->m_Data[(i + 1) * size + 8];

					p3.x = vbo->m_Data[(i + 2) * size];
					p3.y = vbo->m_Data[(i + 2) * size + 1];
					p3.z = vbo->m_Data[(i + 2) * size + 2];
					n3.x = vbo->m_Data[(i + 2) * size + 6];
					n3.y = vbo->m_Data[(i + 2) * size + 7];
					n3.z = vbo->m_Data[(i + 2) * size + 8];

					// Calculate the face normal
					glm::vec3 faceNormal = glm::normalize(glm::cross(p2 - p1, p3 - p1));
					n1 += faceNormal; n1 = glm::normalize(n1);
					n2 += faceNormal; n2 = glm::normalize(n2);
					n3 += faceNormal; n3 = glm::normalize(n3);

					vbo->m_Data[(i + 0) * size + 6] = n1.x;
					vbo->m_Data[(i + 0) * size + 7] = n1.y;
					vbo->m_Data[(i + 0) * size + 8] = n1.z;

					vbo->m_Data[(i + 1) * size + 6] = n1.x;
					vbo->m_Data[(i + 1) * size + 7] = n1.y;
					vbo->m_Data[(i + 1) * size + 8] = n1.z;

					vbo->m_Data[(i + 2) * size + 6] = n1.x;
					vbo->m_Data[(i + 2) * size + 7] = n1.y;
					vbo->m_Data[(i + 2) * size + 8] = n1.z;
				}
			}
		}
	}

	KLS_AABBBox& KLS_MeshBuffer::updateBoundingBox()
	{
		if (m_VAO)
		{
			for (int i = 0; i < m_VAO->getVertexBufferCount(); ++i)
			{
				KLS_VertexBuffer* vbo = m_VAO->getVertexBuffer(i);

				int count = (int)vbo->m_Data.size() / getVertexSize(vbo->getVertexType());
				for (int v = 0; v < count; v += getVertexSize(vbo->getVertexType()))
				{
					glm::vec3 p;
					p.x = vbo->m_Data[v];
					p.y = vbo->m_Data[v + 1];
					p.z = vbo->m_Data[v + 2];
					m_BoundingBox.addPoint(p);
				}
			}
		}
		return m_BoundingBox;
	}

} // end namespace