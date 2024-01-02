// include the needed header files
#include "KLS_OpenGL_Buffer.h"
#include "KLS_Logger.h"
#include <glad/gl.h>

// unclutter the global namespace
namespace KLS
{
	// Generic uniform buffer
	// Base class for managing uniform buffer data

	// Constructor: Allocates memory for the buffer.
	KLS_OpenGL_UniformBuffer::KLS_OpenGL_UniformBuffer(std::size_t bufferSize)
		: KLS_UniformBuffer(bufferSize)
	{
		glGenBuffers(1, &m_BufferID); CHECK_GL_ERROR;
		allocateStorage(); CHECK_GL_ERROR;
	}

	// Destructor: Frees the allocated memory.
	KLS_OpenGL_UniformBuffer::~KLS_OpenGL_UniformBuffer()
	{  
		glDeleteBuffers(1, &m_BufferID); CHECK_GL_ERROR;
	}

	// Set data into the buffer.
	void KLS_OpenGL_UniformBuffer::setData(const void* data, std::size_t size)
	{ 
		KLS_UniformBuffer::setData(data, size);
	}

	// Bind the uniform buffer 
	void KLS_OpenGL_UniformBuffer::bind() const
	{  
		KLS_UniformBuffer::bind();
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID); CHECK_GL_ERROR;
	}

	// Unbind the uniform buffer 
	void KLS_OpenGL_UniformBuffer::unbind() const
	{  
		KLS_UniformBuffer::unbind();
		glBindBuffer(GL_UNIFORM_BUFFER, 0); CHECK_GL_ERROR;
	}

	// Set the binding point 
	void KLS_OpenGL_UniformBuffer::setBindingPoint(uint32_t bindingPoint)
	{  
		KLS_UniformBuffer::setBindingPoint(bindingPoint);
		this->m_BindingPoint = bindingPoint;
	}

	// Allocate storage for the uniform buffer 
	void KLS_OpenGL_UniformBuffer::allocateStorage()
	{ 
		KLS_UniformBuffer::allocateStorage();
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID); CHECK_GL_ERROR;
		glBufferData(GL_UNIFORM_BUFFER, getBufferSize(), getData(), GL_STATIC_DRAW); CHECK_GL_ERROR;
		//			glBindBuffer(GL_UNIFORM_BUFFER, 0); CHECK_GL_ERROR;
	}

	// Update the data in the uniform buffer 
	void KLS_OpenGL_UniformBuffer::updateData()
	{  
		KLS_UniformBuffer::updateData();
		glBindBuffer(GL_UNIFORM_BUFFER, m_BufferID); CHECK_GL_ERROR;
		glBufferSubData(GL_UNIFORM_BUFFER, 0, getBufferSize(), getData()); CHECK_GL_ERROR;
		//			glBindBuffer(GL_UNIFORM_BUFFER, 0); CHECK_GL_ERROR;
	}

	// Bind a range of the uniform buffer to a specific binding point 
	void KLS_OpenGL_UniformBuffer::bindBufferRange(std::size_t offset, std::size_t size) const
	{  
		KLS_UniformBuffer::bindBufferRange(offset, size);
		glBindBufferRange(GL_UNIFORM_BUFFER, m_BindingPoint, m_BufferID, offset, size);
	}

	// Bind the uniform buffer to a specific binding point.
	void KLS_OpenGL_UniformBuffer::bindBufferBase() const
	{  
		KLS_UniformBuffer::bindBufferBase();
		glBindBufferBase(GL_UNIFORM_BUFFER, m_BindingPoint, m_BufferID); CHECK_GL_ERROR;
	}

	void KLS_OpenGL_UniformBuffer::prepare(const void* data, std::size_t size, int binding)
	{  
		KLS_UniformBuffer::prepare(data, size, binding);
		setData(data, size); CHECK_GL_ERROR;
		updateData(); CHECK_GL_ERROR;
		setBindingPoint(binding); CHECK_GL_ERROR;
		bindBufferBase(); CHECK_GL_ERROR;
	}

	/*
	* vertexbuffer interface class
	* all vertex buffer data is considered to be floats
	* for example : a glm::vec3 is stored as float,float,float
	*/
		// class constructor / destructor
	KLS_OpenGL_VertexBuffer::KLS_OpenGL_VertexBuffer()
		: KLS_VertexBuffer()
	{  
		// create the buffer
		glGenBuffers(1, &m_RendererID);
	}

	KLS_OpenGL_VertexBuffer::~KLS_OpenGL_VertexBuffer()
	{  
		// destroy the buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	// send the data to the gpu
	void KLS_OpenGL_VertexBuffer::bind()
	{  
		KLS_VertexBuffer::bind();
		// bind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}

	// reset the gpu data
	void KLS_OpenGL_VertexBuffer::unbind()
	{
		KLS_VertexBuffer::unbind();
		// unbind the buffer
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	// rebuild the data
	void KLS_OpenGL_VertexBuffer::setData(KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, const std::vector<float>& data)
	{  
		KLS_VertexBuffer::setData(vertextype, layout, data);
		bind();
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_DYNAMIC_DRAW);
	}

	// update the data (must be the same size as it was at initialization time)
	void KLS_OpenGL_VertexBuffer::updateData(const std::vector<float>& data)
	{
		KLS_VertexBuffer::updateData(data);
		bind();
		glBufferSubData(GL_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
	}

	/*
	* indexbuffer interface class
	* all index buffer data is considered to be uint32_t
	* for example : unint32_t,unint32_t,unint32_t,unint32_t...
	*/
		// class contructor / destructor
	KLS_OpenGL_IndexBuffer::KLS_OpenGL_IndexBuffer()
		: KLS_IndexBuffer()
	{ 
		// create the buffer
		glGenBuffers(1, &m_RendererID);
	}
	
	KLS_OpenGL_IndexBuffer::~KLS_OpenGL_IndexBuffer()
	{
		// destroy the buffer
		glDeleteBuffers(1, &m_RendererID);
	}

	// send the data to the gpu
	void KLS_OpenGL_IndexBuffer::bind() 
	{
		KLS_IndexBuffer::bind();
		// bind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}

	// reset the gpu data
	void KLS_OpenGL_IndexBuffer::unbind() 
	{
		KLS_IndexBuffer::unbind();
		// unbind the buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	// rebuild the data
	void KLS_OpenGL_IndexBuffer::setData(const std::vector<uint32_t>& data)
	{
		KLS_IndexBuffer::setData(data);
		bind();
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size() * sizeof(float), data.data(), GL_STATIC_DRAW);
	}

	// update the data (must be the same size as it was at initialization time)
	void KLS_OpenGL_IndexBuffer::updateData(const std::vector<uint32_t>& data) 
	{ 
		KLS_IndexBuffer::updateData(data);
		bind();
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, data.size() * sizeof(float), data.data());
	}

	// vertexarray class
	// holds list of vertexbuffers and a single indexbuffer
	// when vertexbuffer is added, this class parses the vertexbuffer layout and sets up apprpriately
		// class constructors
	KLS_OpenGL_VertexArray::KLS_OpenGL_VertexArray()
		: KLS_VertexArray()
	{ 
		// create the buffer
		glGenVertexArrays(1, &m_RendererID);
	}

	// class destructors
	KLS_OpenGL_VertexArray::~KLS_OpenGL_VertexArray()
	{ 
		// destory the buffer
		glDeleteVertexArrays(1, &m_RendererID);
	}

	// send the data to the gpu
	void KLS_OpenGL_VertexArray::bind()
	{
		KLS_VertexArray::bind();
		// bind the buffer
		glBindVertexArray(m_RendererID);
	}

	// reset the gpu data
	void KLS_OpenGL_VertexArray::unbind()
	{
		KLS_VertexArray::unbind();
		// unbind the buffer
		glBindVertexArray(0);
	}

	// set the index buffer
	void KLS_OpenGL_VertexArray::setIndexBuffer(KLS_IndexBuffer* ibo)
	{
		KLS_VertexArray::setIndexBuffer(ibo);
		bind();
		GLuint buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ibo->getData().size() * sizeof(unsigned int), ibo->getData().data(), GL_STATIC_DRAW);
	}

	///// vertex array - convert our data type to opengl data type
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		}
		return 0;
	}

	// add a vertexbuffer to the VAo
	void KLS_OpenGL_VertexArray::addVertexBuffer(KLS_VertexBuffer* vbo)
	{
		KLS_VertexArray::addVertexBuffer(vbo);

		bind();
		vbo->bind();

		auto layout = vbo->getLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(m_VertexBufferIndex,
					element.getComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.getStride(),
					(const void*)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.getComponentCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.getStride(),
						(const void*)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			}
		}
	}

} // end namespace
