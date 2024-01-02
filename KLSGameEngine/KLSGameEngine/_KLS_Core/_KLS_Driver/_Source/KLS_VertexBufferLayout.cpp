// include the needed header files
#include "KLS_VertexBufferLayout.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor
	BufferElement::BufferElement(ShaderDataType type, const std::string& name, bool normalized)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
	{
	}

	// get the stored component count
	uint32_t BufferElement::getComponentCount() const
	{
		switch (Type)
		{
			case ShaderDataType::Float:   return 1;
			case ShaderDataType::Float2:  return 2;
			case ShaderDataType::Float3:  return 3;
			case ShaderDataType::Float4:  return 4;
			case ShaderDataType::Mat3:    return 3; // 3* float3
			case ShaderDataType::Mat4:    return 4; // 4* float4
		}
		return 0;
	}

	// get the stored component type size
	uint32_t BufferElement::ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:    return 4;
			case ShaderDataType::Float2:   return 4 * 2;
			case ShaderDataType::Float3:   return 4 * 3;
			case ShaderDataType::Float4:   return 4 * 4;
			case ShaderDataType::Mat3:     return 4 * 3 * 3;
			case ShaderDataType::Mat4:     return 4 * 4 * 4;
		}
		return 0;
	}


	// class constuctors
	KLS_VertexBufferLayout::KLS_VertexBufferLayout() {}
	KLS_VertexBufferLayout::KLS_VertexBufferLayout(bool dynamic, std::initializer_list<BufferElement> elements)
		: m_Dynamic(dynamic), m_Elements(elements)
	{
		calculateOffsetsAndStride();
	}

	// distance betwen entires
	uint32_t KLS_VertexBufferLayout::getStride() const 
	{ 
		return m_Stride; 
	}

	// list of elements
	const std::vector<BufferElement>& KLS_VertexBufferLayout::getElements() const 
	{ 
		return m_Elements; 
	}

	// beginning of list
	std::vector<BufferElement>::iterator KLS_VertexBufferLayout::begin()
	{ 
		return m_Elements.begin(); 
	}

	// end of list
	std::vector<BufferElement>::iterator KLS_VertexBufferLayout::end()
	{ 
		return m_Elements.end(); 
	}

	// beginning of list
	std::vector<BufferElement>::const_iterator KLS_VertexBufferLayout::begin() const
	{ 
		return m_Elements.begin(); 
	}

	// end of list
	std::vector<BufferElement>::const_iterator KLS_VertexBufferLayout::end() const
	{ 
		return m_Elements.end(); 
	}

	// calculate the offset and stride of the list
	void KLS_VertexBufferLayout::calculateOffsetsAndStride()
	{
		size_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	// the overall size of a specified vertex type
	uint32_t getVertexSize(KLS_VERTEX_TYPE type)
	{
		switch (type)
		{
		case KLS_VERTEX_TYPE::KLSVT_NONE: return 0; break;
		case KLS_VERTEX_TYPE::KLSVT2D: return sizeof(float) * 4; break;
		case KLS_VERTEX_TYPE::KLSVT3D: return sizeof(float) * 8; break;
		case KLS_VERTEX_TYPE::KLSVT3D_ANIM: return sizeof(float) * 22; break;
		case KLS_VERTEX_TYPE::KLSVT_PARTICLE: return sizeof(float) * 9; break;
		case KLS_VERTEX_TYPE::KLSVT_INSTANCED: return sizeof(float) * 16; break;
		}
		return 0;
	}

	// a few prebuilt layouts
	// the full layout for the specified vertex type
	KLS_VertexBufferLayout getVertexBufferLayout(KLS_VERTEX_TYPE type, bool dynamic)
	{
		switch (type)
		{
		case KLS_VERTEX_TYPE::KLSVT2D:
		{
			return KLS_VertexBufferLayout{ dynamic, {
				{ ShaderDataType::Float2, "inputPosition" },
				{ ShaderDataType::Float2, "inputTexCoord" }
			} };
		} break;
		case KLS_VERTEX_TYPE::KLSVT3D:
		{
			return KLS_VertexBufferLayout{ dynamic, {
				{ ShaderDataType::Float3, "inputPosition" },
				{ ShaderDataType::Float2, "inputTexCoord" },
				{ ShaderDataType::Float3, "inputNormal" }
			} };
		} break;
		case KLS_VERTEX_TYPE::KLSVT3D_ANIM:
		{
			return KLS_VertexBufferLayout{ dynamic, {
				{ ShaderDataType::Float3, "inputPosition" },
				{ ShaderDataType::Float2, "inputTexCoord" },
				{ ShaderDataType::Float3, "inputNormal" },
				{ ShaderDataType::Float4, "inputBoneId" },
				{ ShaderDataType::Float4, "inputBoneWeights" }
			} };
		} break;
		case KLS_VERTEX_TYPE::KLSVT_PARTICLE:
		{
			return KLS_VertexBufferLayout{ dynamic, {
				{ ShaderDataType::Float3, "inputPosition" },
				{ ShaderDataType::Float2, "inputSize" },
				{ ShaderDataType::Float4, "inputColor" }

			} };
		} break;
		case KLS_VERTEX_TYPE::KLSVT_INSTANCED:
		{
			return KLS_VertexBufferLayout{ dynamic, {
				{ ShaderDataType::Mat4, "aInstanceMatrix" }
			} };
		} break;
		}

		return KLS_VertexBufferLayout();
	}

} // end namespace
