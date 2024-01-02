// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Colors.h"
#include "KLS_MeshBuffer.h"
#include "KLS_AABBBox.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;

	// simple mesh class that holds multiple meshbuffers
	class KLS_API KLS_Mesh
	{
	public:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);  // pointer to the driver instance
		KLS_VARIABLE(KLS_AABBBox, BoundingBox, KLS_AABBBox(glm::vec3(-.5f), glm::vec3(.5f)));
		KLS_VARIABLE_SETGET(std::string, Name, "");

		// list of meshbuffers
		std::vector<KLS_MeshBuffer*> m_MeshBuffers;
	public:

		// default constructor
		KLS_Mesh(KLS_Driver* driver);

		// creates a fully functional mesh object with one meshbuffer
		KLS_Mesh(KLS_Driver* driver, KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, std::vector<float>& vertexData, std::vector<uint32_t>& indexData);

		// class destructor
		~KLS_Mesh();

		// add a meshbuffer to the list
		KLS_MeshBuffer* addMeshBuffer(KLS_MeshBuffer* mb);

		// get a pointer to the desired meshbuffer
		KLS_MeshBuffer* getMeshBuffer(int index);

		// get the number of meshbuffers in the list
		uint32_t getMeshBufferCount() { return (uint32_t)m_MeshBuffers.size(); }

		// material manipulation methods
		KLS_Material& getMaterial(int index);
		int getMaterialCount();

		KLS_AABBBox& getBoundingBox() { return m_BoundingBox; }
		KLS_AABBBox& updateBoundingBox();

		bool hasTransparency();

		void recalculateNormals();
	};

} // end namespace