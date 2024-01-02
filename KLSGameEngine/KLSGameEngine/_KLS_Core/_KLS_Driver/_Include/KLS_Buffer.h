// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_VertexBufferLayout.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;

	// Generic uniform buffer
	// Base class for managing uniform buffer data
	class KLS_UniformBuffer
	{
	public:
		KLS_VARIABLE_SETGET(uint32_t, BufferID,0);	// id (for opengl calls)
		char* m_Buffer;								// the data buffer		
		std::size_t m_BufferSize;					// the buffer size
		uint32_t m_BindingPoint;
	public:
		// Constructor: Allocates memory for the buffer.
		KLS_UniformBuffer(std::size_t bufferSize);

		// Destructor: Frees the allocated memory.
		virtual ~KLS_UniformBuffer();

		// Set data into the buffer.
		virtual void setData(const void* data, std::size_t size);

		// Get a pointer to the buffer's data.
		virtual const void* getData() const;

		// Get the buffer size.
		virtual std::size_t getBufferSize() const;

		// Bind the uniform buffer 
		virtual void bind() const;

		// Unbind the uniform buffer 
		virtual void unbind() const;

		// Set the binding point 
		virtual void setBindingPoint(uint32_t bindingPoint);

		// Allocate storage for the uniform buffer 
		virtual void allocateStorage();

		// Update the data in the uniform buffer 
		virtual void updateData();

		// Bind a range of the uniform buffer to a specific binding point 
		virtual void bindBufferRange(std::size_t offset, std::size_t size) const;

		// Bind the uniform buffer to a specific binding point.
		virtual void bindBufferBase() const;

		virtual void prepare(const void* data, std::size_t size, int binding);
	};

	/*
	* vertexbuffer interface class
	* all vertex buffer data is considered to be floats
	* for example : a glm::vec3 is stored as float,float,float
	*/
	class KLS_API KLS_VertexBuffer
	{
	private:
		KLS_VARIABLE_GET(KLS_VERTEX_TYPE, VertexType, KLSVT_NONE);					// simple type for anyone that wants to know
		KLS_VARIABLE_GET(KLS_VertexBufferLayout, Layout, KLS_VertexBufferLayout());	// the layout of this buffer
		KLS_VARIABLE_GET(uint32_t, RendererID, -1);									// the rendererId (for opengl function calls)
		std::vector<float> m_Data;													// the actual data all in float format

	public:
		// class constructor / destructor
		KLS_VertexBuffer();
		virtual ~KLS_VertexBuffer();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// rebuild the data
		virtual void setData(KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, const std::vector<float>& data);

		// update the data (must be the same size as it was at initialization time)
		virtual void updateData(const std::vector<float>& data);

		// get the data
		const std::vector<float>& getData() const;
	};

	/*
	* indexbuffer interface class
	* all index buffer data is considered to be uint32_t
	* for example : unint32_t,unint32_t,unint32_t,unint32_t...
	*/
	class KLS_API KLS_IndexBuffer
	{
	private:
		KLS_VARIABLE_GET(uint32_t, RendererID, -1);		// the rendererId (for opengl function calls)
		std::vector<uint32_t> m_Data;					// the actual data in uint32_t format

	public:
		// class contructor / destructor
		KLS_IndexBuffer();
		virtual ~KLS_IndexBuffer();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// rebuild the data
		virtual void setData(const std::vector<uint32_t>& data);

		// update the data (must be the same size as it was at initialization time)
		virtual void updateData(const std::vector<uint32_t>& data);

		// get the data
		const std::vector<uint32_t>& getData() const;

		// get the number of indices
		int getCount();
	};

	// vertexarray class
	// holds list of vertexbuffers and a single indexbuffer
	// when vertexbuffer is added, this class parses the vertexbuffer layout and sets up apprpriately
	class KLS_API KLS_VertexArray
	{
	private:
		KLS_VARIABLE_GET(uint32_t, RendererID, -1);					// the rendererId for opengl function calls
		KLS_VARIABLE_GET(KLS_IndexBuffer*, IndexBuffer, nullptr);	// the single indexbuffer
		std::vector<KLS_VertexBuffer*> m_VertexBuffers;				// the list of vertexbuffers
		int m_VertexBufferIndex = 0;								// variable used in addVertexBuffer call 

	public:
		// class constructors
		KLS_VertexArray();

		// class destructors
		virtual ~KLS_VertexArray();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// set the index buffer
		virtual void setIndexBuffer(KLS_IndexBuffer* ibo);

		// add a vertexbuffer to the VAo
		virtual void addVertexBuffer(KLS_VertexBuffer* vbo);

		// get the numeber of vertexbuffers
		virtual int getVertexBufferCount();

		// get a buffer at index
		virtual KLS_VertexBuffer* getVertexBuffer(int index);
	};
} // end namespace
