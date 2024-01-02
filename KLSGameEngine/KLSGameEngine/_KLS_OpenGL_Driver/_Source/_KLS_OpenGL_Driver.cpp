// include the needed header files
#include "_KLS_OpenGL_Driver.h"
#include "KLS_Logger.h"
#include "KLS_Window.h"
#include "KLS_OpenGL_Buffer.h"
#include "KLS_OpenGL_Texture.h"
#include "KLS_OpenGL_Texture_Cubemap.h"
#include "KLS_OpenGL_Pipeline.h"
#include "KLS_OpenGL_FrameBuffer.h"

#include <GLFW/glfw3.h>

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_OpenGL_Driver::KLS_OpenGL_Driver() : KLS_Driver()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_Driver::KLS_OpenGL_Driver()");
	}

	KLS_OpenGL_Driver::~KLS_OpenGL_Driver()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_Driver::~KLS_OpenGL_Driver()");
	}

	// dual creation allows for better error handling
	bool KLS_OpenGL_Driver::create(KLS_Application* app)
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_Driver::create()");

		// make sure to set the driver type
		setDriverType(KLSDT_OPENGL);

		// call the base class
		KLS_CHECK(KLS_Driver::create(app), "base class creation failed", KLS_FAILURE);

		int version = gladLoadGL(glfwGetProcAddress);
		KLS_CHECK((version != 0), "Failed to initialize OpenGL context", KLS_FAILURE);

		glEnable(GL_MULTISAMPLE);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

#ifdef KLS_OPENGL_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

		// Set the depth buffer to be entirely cleared to 1.0 values.
		glClearDepth(1.0f);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_OpenGL_Driver::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_OpenGL_Driver::cleanup()");

		// call the base class
		return KLS_Driver::cleanup();
	}

	bool KLS_OpenGL_Driver::onSystemWindowMoved(const KLS_Event& e)
	{
		return KLS_Driver::onSystemWindowMoved(e);
	}

	bool KLS_OpenGL_Driver::onSystemWindowSized(const KLS_Event& e)
	{
		return KLS_Driver::onSystemWindowSized(e);
	}

	bool KLS_OpenGL_Driver::onSystemWindowFocused(const KLS_Event& e)
	{
		return KLS_Driver::onSystemWindowFocused(e);
	}

	bool KLS_OpenGL_Driver::onSystemWindowIconified(const KLS_Event& e)
	{
		return KLS_Driver::onSystemWindowIconified(e);
	}

	bool KLS_OpenGL_Driver::onSystemWindowMaximize(const KLS_Event& e)
	{
		return KLS_Driver::onSystemWindowMaximize(e);
	}

	bool KLS_OpenGL_Driver::onSystemFrameBufferResized(const KLS_Event& e)
	{
		return KLS_Driver::onSystemFrameBufferResized(e);
	}

	// instantiated structures are not valid for use
	// instantiated structures are not valid for use
	KLS_UniformBuffer* KLS_OpenGL_Driver::instantiateUniformBuffer(size_t buffersize) { return new KLS_OpenGL_UniformBuffer(buffersize); }
	KLS_VertexBuffer* KLS_OpenGL_Driver::instantiateVertexBuffer() { return new KLS_OpenGL_VertexBuffer(); }
	KLS_IndexBuffer* KLS_OpenGL_Driver::instantiateIndexBuffer() { return new KLS_OpenGL_IndexBuffer(); }
	KLS_VertexArray* KLS_OpenGL_Driver::instantiateVertexArray() { return new KLS_OpenGL_VertexArray(); }
	KLS_Texture* KLS_OpenGL_Driver::instantiateTexture() { return new KLS_OpenGL_Texture(); }
	KLS_Texture_Cubemap* KLS_OpenGL_Driver::instantiateCubemap() { return new KLS_OpenGL_Texture_Cubemap(); }
	KLS_Pipeline* KLS_OpenGL_Driver::instantiatePipeline() { return new KLS_OpenGL_Pipeline(); }
	KLS_FrameBuffer* KLS_OpenGL_Driver::instantiateFrameBuffer() { return new KLS_OpenGL_FrameBuffer(); }

} // end namespace