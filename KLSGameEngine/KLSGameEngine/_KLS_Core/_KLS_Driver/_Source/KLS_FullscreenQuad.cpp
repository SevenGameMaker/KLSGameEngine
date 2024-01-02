#include "KLS_FullscreenQuad.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_Mesh.h"
#include "KLS_Buffer.h"
#include "KLS_Geometry.h"

namespace KLS
{
	KLS_FullscreenQuad::KLS_FullscreenQuad(KLS_Driver* driver)
	{
		// log this activity
		KLS_TRACE("KLS_FullscreenQuad::KLS_FullscreenQuad");

		m_Driver = driver;

		std::vector<float> vertexData = createVertices_Quad2D(1.0f);
		std::vector<uint32_t> indexData = createIndices_Quad2D();

		m_Quad = new KLS_Mesh(m_Driver, KLSVT2D, getVertexBufferLayout(KLSVT2D, false), vertexData, indexData);
			m_Quad->getMaterial(0).MaterialType = KLSMT_FSQUAD;
			m_Quad->getMaterial(0).ColorDiffuse = KLS_WHITE;
			m_Quad->getMaterial(0).Wireframe = false;
	}

	KLS_FullscreenQuad::~KLS_FullscreenQuad()
	{
		// log this activity
		KLS_TRACE("KLS_FullscreenQuad::~KLS_FullscreenQuad");

		if (m_Quad) delete(m_Quad); m_Quad = nullptr;
	}

	void KLS_FullscreenQuad::setTexture(std::string name)
	{
		m_Quad->getMaterial(0).Texture_Diffuse[0] = name;
	}

	void KLS_FullscreenQuad::setMaterialType(KLS_MATERIAL_TYPE type)
	{
		m_Quad->getMaterial(0).MaterialType = type;
	}

	void KLS_FullscreenQuad::draw()
	{
		glm::mat4 t(1);
		if (m_Quad)	getDriver()->drawMesh(m_Quad,t);
	}

} // end namespace