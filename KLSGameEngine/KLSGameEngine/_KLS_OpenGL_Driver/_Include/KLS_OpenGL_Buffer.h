// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Buffer.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;

	// Generic uniform buffer
	// Base class for managing uniform buffer data
	class KLS_OpenGL_UniformBuffer : public KLS_UniformBuffer
	{
	public:

	public:
		// Constructor: Allocates memory for the buffer.
		KLS_OpenGL_UniformBuffer(std::size_t bufferSize);

		// Destructor: Frees the allocated memory.
		virtual ~KLS_OpenGL_UniformBuffer();

		// Set data into the buffer.
		virtual void setData(const void* data, std::size_t size);

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
	class KLS_API KLS_OpenGL_VertexBuffer : public KLS_VertexBuffer
	{
	private:
	public:
		// class constructor / destructor
		KLS_OpenGL_VertexBuffer();
		virtual ~KLS_OpenGL_VertexBuffer();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// rebuild the data
		virtual void setData(KLS_VERTEX_TYPE vertextype, KLS_VertexBufferLayout layout, const std::vector<float>& data);

		// update the data (must be the same size as it was at initialization time)
		virtual void updateData(const std::vector<float>& data);
	};

	/*
	* indexbuffer interface class
	* all index buffer data is considered to be uint32_t
	* for example : unint32_t,unint32_t,unint32_t,unint32_t...
	*/
	class KLS_API KLS_OpenGL_IndexBuffer : public KLS_IndexBuffer
	{
	private:

	public:
		// class contructor / destructor
		KLS_OpenGL_IndexBuffer();
		virtual ~KLS_OpenGL_IndexBuffer();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// rebuild the data
		virtual void setData(const std::vector<uint32_t>& data);

		// update the data (must be the same size as it was at initialization time)
		virtual void updateData(const std::vector<uint32_t>& data);
	};

	// vertexarray class
	// holds list of vertexbuffers and a single indexbuffer
	// when vertexbuffer is added, this class parses the vertexbuffer layout and sets up apprpriately
	class KLS_API KLS_OpenGL_VertexArray : public KLS_VertexArray
	{
	private:
	public:
		// class constructors
		KLS_OpenGL_VertexArray();

		// class destructors
		virtual ~KLS_OpenGL_VertexArray();

		// send the data to the gpu
		virtual void bind();

		// reset the gpu data
		virtual void unbind();

		// set the index buffer
		virtual void setIndexBuffer(KLS_IndexBuffer* ibo);

		// add a vertexbuffer to the VAo
		virtual void addVertexBuffer(KLS_VertexBuffer* vbo);
	};

} // end namespace
