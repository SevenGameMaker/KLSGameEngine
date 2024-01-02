// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "_KLS_Driver.h"

// unclutter the global namespace
namespace KLS
{
	/*
	*
	*/
	class KLS_API KLS_OpenGL_Driver : public KLS_Driver
	{
	private:

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_OpenGL_Driver();
		virtual ~KLS_OpenGL_Driver();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// event overrides to provide functionality
		virtual bool onSystemWindowMoved(const KLS_Event& e);
		virtual bool onSystemWindowSized(const KLS_Event& e);
		virtual bool onSystemWindowFocused(const KLS_Event& e);
		virtual bool onSystemWindowIconified(const KLS_Event& e);
		virtual bool onSystemWindowMaximize(const KLS_Event& e);
		virtual bool onSystemFrameBufferResized(const KLS_Event& e);

		// render methods
		virtual void beginFrame();
		virtual void endFrame();
		virtual void setClearColor(glm::vec4 color);
		virtual void setViewport(int width, int height);
		virtual void resetFBO(KLS_CLEARFLAG flags);
		virtual void clear(KLS_CLEARFLAG flags);
		virtual void setDepth(bool value);
		virtual void setTransparency(bool value);

		virtual void drawMesh(KLS_Mesh* mesh, glm::mat4& transform);

		// instantiated structures are not valid for use
		virtual KLS_UniformBuffer* instantiateUniformBuffer(size_t buffersize);
		virtual KLS_VertexBuffer* instantiateVertexBuffer();
		virtual KLS_IndexBuffer* instantiateIndexBuffer();
		virtual KLS_VertexArray* instantiateVertexArray();
		virtual KLS_Texture* instantiateTexture();
		virtual KLS_Texture_Cubemap* instantiateCubemap();
		virtual KLS_Pipeline* instantiatePipeline();
		virtual KLS_FrameBuffer* instantiateFrameBuffer();
	};


} // end namespace