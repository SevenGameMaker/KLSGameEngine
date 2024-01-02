// include the needed header files
#include "KLS_Buffer.h"
#include "KLS_Logger.h"

// unclutter the global namespace
namespace KLS
{
	// Generic uniform buffer
	// Base class for managing uniform buffer data

	// Constructor: Allocates memory for the buffer.
	KLS_UniformBuffer::KLS_UniformBuffer(std::size_t bufferSize)
		: m_Buffer(nullptr), m_BufferSize(bufferSize), m_BufferID(0)
	{
		m_Buffer = new char[bufferSize];
	}

	// Destructor: Frees the allocated memory.
	KLS_UniformBuffer::~KLS_UniformBuffer()
	{  
		delete[] m_Buffer;
	}

	// Set data into the buffer.
	void KLS_UniformBuffer::setData(const void* data, std::size_t size)
	{ 
		if (size <= m_BufferSize) {
			std::memcpy(m_Buffer, data, size);
		}
		else KLS_ERROR("wrong buffer size!!!!!!!!!!!!!!!!!");
	}

	// Get a pointer to the buffer's data.
	const void* KLS_UniformBuffer::getData() const
	{  
		return m_Buffer;
	}

	// Get the buffer size.
	std::size_t KLS_UniformBuffer::getBufferSize() const
	{  
		return m_BufferSize;
	}

	// Bind the uniform buffer 
	void KLS_UniformBuffer::bind() const
	{  
	}

	// Unbind the uniform buffer 
	void KLS_UniformBuffer::unbind() const
	{  
	}

	// Set the binding point 
	void KLS_UniformBuffer::setBindingPoint(uint32_t bindingPoint)
	{  
	}

	// Allocate storage for the uniform buffer 
	void KLS_UniformBuffer::allocateStorage()
	{ 
	}

	// Update the data in the uniform buffer 
	void KLS_UniformBuffer::updateData()
	{  
	}

	// Bind a range of the uniform buffer to a specific binding point 
	void KLS_UniformBuffer::bindBufferRange(std::size_t offset, std::size_t size) const
	{  
	}

	// Bind the uniform buffer to a specific binding point.
	void KLS_UniformBuffer::bindBufferBase() const
	{  
	}

	void KLS_UniformBuffer::prepare(const void* data, std::size_t size, int binding)
	{  
	}

	/*
	* vertexbuffer interface class
	* all vertex buffer data is considered to be floats
	* for example : a glm::vec3 is stored as float,float,float
	*/
		// class constructor / destructor
	KLS_VertexBuffer::KLS_VertexBuffer()
	{  
	}

	KLS_VertexBuffer::~KLS_VertexBuffer()
	{  
		m_Data.clear();
	}

	// send the data to the gpu
	void KLS_VertexBuffer::bind()
	{  
	}

	// reset the gpu data
	void KLS_VertexBuffer::unbind(){  }

	// rebuild the data
	void KLS_VertexBuffer::setData(KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, const std::vector<float>& data)
	{  
		m_VertexType = vertextype;
		m_Layout = layout;
		m_Data = data;
	}

	// update the data (must be the same size as it was at initialization time)
	void KLS_VertexBuffer::updateData(const std::vector<float>& data)
	{
		m_Data = data;
	}

	// get the data
	const std::vector<float>& KLS_VertexBuffer::getData() const 
	{ 
		return m_Data; 
	}

	/*
	* indexbuffer interface class
	* all index buffer data is considered to be uint32_t
	* for example : unint32_t,unint32_t,unint32_t,unint32_t...
	*/
		// class contructor / destructor
	KLS_IndexBuffer::KLS_IndexBuffer()
	{ 
	}
	
	KLS_IndexBuffer::~KLS_IndexBuffer()
	{
		m_Data.clear();
	}

	// send the data to the gpu
	void KLS_IndexBuffer::bind() 
	{
	}

	// reset the gpu data
	void KLS_IndexBuffer::unbind() 
	{
	}

	// rebuild the data
	void KLS_IndexBuffer::setData(const std::vector<uint32_t>& data)
	{
		m_Data = data;
	}

	// update the data (must be the same size as it was at initialization time)
	void KLS_IndexBuffer::updateData(const std::vector<uint32_t>& data) 
	{ 
		m_Data = data;
	}

	// get the data
	const std::vector<uint32_t>& KLS_IndexBuffer::getData() const
	{
		return m_Data;
	}

	// get the number of indices
	int KLS_IndexBuffer::getCount()
	{
		return (int)m_Data.size();
	}

	// vertexarray class
	// holds list of vertexbuffers and a single indexbuffer
	// when vertexbuffer is added, this class parses the vertexbuffer layout and sets up apprpriately
		// class constructors
	KLS_VertexArray::KLS_VertexArray()
	{ 
	}

	// class destructors
	KLS_VertexArray::~KLS_VertexArray()
	{ 
		for (KLS_VertexBuffer* vb : m_VertexBuffers) { delete vb; }
		m_VertexBuffers.clear();
		if (m_IndexBuffer) delete(m_IndexBuffer); m_IndexBuffer = nullptr;
	}

	// send the data to the gpu
	void KLS_VertexArray::bind()
	{
	}

	// reset the gpu data
	void KLS_VertexArray::unbind()
	{
	}

	// set the index buffer
	void KLS_VertexArray::setIndexBuffer(KLS_IndexBuffer* ibo)
	{
		if (m_IndexBuffer) delete(m_IndexBuffer); m_IndexBuffer = ibo;
	}

	// add a vertexbuffer to the VAo
	void KLS_VertexArray::addVertexBuffer(KLS_VertexBuffer* vbo)
	{
		m_VertexBuffers.push_back(vbo);
	}

	// get the numeber of vertexbuffers
	int KLS_VertexArray::getVertexBufferCount()
	{
		return (int)m_VertexBuffers.size();
	}

	// get a buffer at index
	KLS_VertexBuffer* KLS_VertexArray::getVertexBuffer(int index)
	{
		return m_VertexBuffers[index];
	}

} // end namespace
