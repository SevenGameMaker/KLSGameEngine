// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_EventHandler.h"
#include "KLS_Colors.h"
#include "KLS_Material.h"

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef class KLS_Application KLS_Application;
	typedef class KLS_Window KLS_Window;
	typedef class KLS_ResourceManager KLS_ResourceManager;
	typedef class KLS_UniformBuffer KLS_UniformBuffer;
	typedef class KLS_VertexBuffer KLS_VertexBuffer;
	typedef class KLS_IndexBuffer KLS_IndexBuffer;
	typedef class KLS_VertexArray KLS_VertexArray;
	typedef class KLS_Texture KLS_Texture;
	typedef class KLS_Texture_Cubemap KLS_TextureCubemap;
	typedef class KLS_Mesh KLS_Mesh;
	typedef class KLS_Pipeline KLS_Pipeline;
	typedef class KLS_FrameBuffer KLS_FrameBuffer;
	typedef class KLS_FrameBufferManager KLS_FrameBufferManager;
	typedef class KLS_Camera KLS_Camera;
	typedef class KLS_Camera_FPS KLS_Camera_FPS;

	/*
	*
	*/
	class KLS_API KLS_Driver : public KLS_EventHandler
	{
	private:
		KLS_VARIABLE_SETGET(KLS_DRIVER_TYPE, DriverType, KLSDT_NONE);
		KLS_VARIABLE_GET(KLS_Application*, Application, nullptr);
		KLS_VARIABLE_GET(KLS_Window*, Window, nullptr);
		KLS_VARIABLE_GET(KLS_ResourceManager*, ResourceManager, nullptr);
		KLS_VARIABLE_GET(KLS_FrameBufferManager*, FrameBufferManager, nullptr);
		KLS_VARIABLE_GET(glm::vec4, ClearColor, KLS_LIGHTBLUE);

		KLS_VARIABLE_GET(KLS_Camera*, DefaultCamera, nullptr);
		KLS_VARIABLE_SETGET(KLS_Camera*, ActiveCamera, nullptr);

		KLS_VARIABLE_GET(KLS_UniformBuffer*, UniformBufferData, nullptr);
		KLS_VARIABLE_GET(KLS_UniformBuffer*, UniformBufferLights, nullptr);
		KLS_VARIABLE_SETGET(int, NumberOfLights, 0);

		// the current pipeline states
		KLS_Material m_Material;
		KLS_Pipeline* m_Pipeline = nullptr;

		// system level frame variables for easier tracking
		KLS_VARIABLE_SETGET(glm::mat4, ViewMatrix, glm::mat4(1));
		KLS_VARIABLE_SETGET(glm::mat4, ProjMatrix, glm::mat4(1));
		KLS_VARIABLE_SETGET(glm::mat4, OrthoMatrix, glm::mat4(1));
		KLS_VARIABLE_SETGET(glm::vec3, CameraPosition, glm::vec3(0));
		KLS_VARIABLE_SETGET(glm::vec3, CameraDirection, glm::vec3(0));
		KLS_VARIABLE_SETGET(glm::vec4, ClipPlane, glm::vec4(0));
		KLS_VARIABLE_SETGET(double, ElapsedTime, 0);
		KLS_VARIABLE_SETGET(double, TotalTime, 0);

		// variables exposed for ImGui ease of access
		KLS_VARIABLE_EDITABLE_SETGET(bool, GPUProtect, true);
		KLS_VARIABLE_EDITABLE_SETGET(int, GPUMaxFrameRate, 100);
		KLS_VARIABLE_EDITABLE_SETGET(bool, UseFBO, true);
		KLS_VARIABLE_EDITABLE_SETGET(bool, UsePostProcessing, false);
		int counter = 0;

		KLS_VARIABLE_EDITABLE_SETGET(glm::vec4, AmbientColor, glm::vec4(1));
		KLS_VARIABLE_EDITABLE_SETGET(float, AmbientIntensity, .1f);

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_Driver();
		virtual ~KLS_Driver();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app);
		virtual bool postCreate();

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// event overrides to provide functionality
		virtual bool onSystemWindowMoved(const KLS_Event& e);
		virtual bool onSystemWindowSized(const KLS_Event& e);
		virtual bool onSystemWindowFocused(const KLS_Event& e);
		virtual bool onSystemWindowIconified(const KLS_Event& e);
		virtual bool onSystemWindowMaximize(const KLS_Event& e);
		virtual bool onSystemFrameBufferResized(const KLS_Event& e);

		// set the current rendering material. updat ethe pipeline if needed
		virtual void setMaterial(const KLS_Material& material);

		// update the camera
		virtual void updateCamera(const double& elapsedtime);
		virtual void updateUniforms();

		// render methods
		virtual void beginFrame();
		virtual void frame(const double &elapsedtime);
		virtual void endFrame();
		virtual void swapBuffers();
		virtual void setClearColor(glm::vec4 color);
		virtual void setViewport(int width, int height);
		virtual void resetFBO(KLS_CLEARFLAG flags);

		virtual void clear(KLS_CLEARFLAG flags);
		virtual void setDepth(bool value);
		virtual void setTransparency(bool value);

		virtual void drawMesh(KLS_Mesh* mesh, glm::mat4& transform);

		// instantiated structures are not valid for use
		virtual KLS_UniformBuffer*		instantiateUniformBuffer(size_t buffersize) { return nullptr; }
		virtual KLS_VertexBuffer*		instantiateVertexBuffer() { return nullptr; }
		virtual KLS_IndexBuffer*		instantiateIndexBuffer() { return nullptr; }
		virtual KLS_VertexArray*		instantiateVertexArray() { return nullptr; }
		virtual KLS_Texture*			instantiateTexture() { return nullptr; }
		virtual KLS_Texture_Cubemap*	instantiateCubemap() { return nullptr; }
		virtual KLS_Pipeline*			instantiatePipeline() { return nullptr; }
		virtual KLS_FrameBuffer*		instantiateFrameBuffer() { return nullptr; }
	};


} // end namespace