// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Buffer.h"
#include "KLS_Material.h"
#include "KLS_AABBBox.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;

	// simple meshbuffer class to hold a single Vertexarray and material
	class KLS_API KLS_MeshBuffer
	{
	public:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);					// pointer to the driver instance
		KLS_VARIABLE_GET(KLS_VertexArray*, VAO, nullptr);				// the vertexarray
		KLS_VARIABLE(KLS_Material, Material,KLS_IDENTITY_MATERIAL);		// the material
		KLS_VARIABLE(KLS_AABBBox, BoundingBox, KLS_IDENTITY_AABBBOX);	// bounding box

	public:
		// default constructor - at minimum, this class needs to knwo the driver
		KLS_MeshBuffer(KLS_Driver* driver);

		// create a fully functional meshbuffer
		KLS_MeshBuffer(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData);

		// class destructor
		~KLS_MeshBuffer();

		// bind / unbins the VAO
		void bind();
		void unbind();

		// add a vertexbuffer to the VAO (automatically determines attributes from vertexbuffer layout variable)
		void addVertexBuffer(KLS_VertexBuffer* vertexBuffer);

		// set the index buffer of the VAO
		void setIndexBuffer(KLS_IndexBuffer* indexBuffer);

		// material interaction
		KLS_Material& getMaterial() { return m_Material; }
		void setMaterial(KLS_Material& material) { m_Material = material; }

		KLS_AABBBox& getBoundingBox() { return m_BoundingBox; }
		KLS_AABBBox& updateBoundingBox();

		virtual void calculateNormals();
	};


} // end namespace
