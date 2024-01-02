// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	/*
	* a single buffer element
	* example : a glm::vec3 would be 1 of these elements
	* and the type would be ShaderDataType::Float3
	*/
	struct KLS_API BufferElement
	{
		std::string		Name = KLS_DEFAULT_STRING;
		ShaderDataType	Type = ShaderDataType::None;
		uint32_t		Size = 0;
		size_t			Offset = 0;
		bool			Normalized = false;

		// class construtors
		BufferElement() = default;
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false);

		// get the stored component count
		uint32_t getComponentCount() const;

		// get the stored component type size
		uint32_t ShaderDataTypeSize(ShaderDataType type);
	};

	/*
	* a vertexbuffer layout description
	* since all vertices are assumed to be floats
	* this structure contains the layout for the gpu to understand
	*/
	class KLS_API KLS_VertexBufferLayout
	{
	private:
		KLS_VARIABLE_SETGET(bool, Dynamic, false);	// is this bufffer static or dynamic in the gpu
		std::vector<BufferElement> m_Elements;		// the list of elements
		uint32_t m_Stride = 0;						// the current stride between elements

	public:
		// class constructors
		KLS_VertexBufferLayout();
		KLS_VertexBufferLayout(bool dynamic, std::initializer_list<BufferElement> elements);

		// distance between entries
		uint32_t getStride() const;

		// return the list of entries
		const std::vector<BufferElement>& getElements() const;

		// return the beginiing and end of the list of entries
		std::vector<BufferElement>::iterator begin();
		std::vector<BufferElement>::iterator end();
		std::vector<BufferElement>::const_iterator begin() const;
		std::vector<BufferElement>::const_iterator end() const;
	private:
		// calcualte the layout offsets and strides
		void calculateOffsetsAndStride();
	};

	// a full layout for the specified vertex type
	KLS_VertexBufferLayout KLS_API getVertexBufferLayout(KLS_VERTEX_TYPE type, bool dynamic);

	// the overall size of a specified vertex type
	uint32_t KLS_API getVertexSize(KLS_VERTEX_TYPE type);

} // end namespace
