// include the needed header files
#include "KLS_ImGui.h"
#include "KLS_Logger.h"
#include "KLS_Window.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_ImGui::KLS_ImGui()
	{
		// log this activity
		KLS_TRACE("KLS_ImGui::KLS_ImGui()");
	}

	KLS_ImGui::~KLS_ImGui()
	{
		// log this activity
		KLS_TRACE("KLS_ImGui::~KLS_ImGui()");
	}

	// dual creation allows for better error handling
	bool KLS_ImGui::create(KLS_Application* app)
	{
		// log this activity
		KLS_TRACE("KLS_ImGui::create()");

		// remember this
		m_Application = app;
		m_DriverType = m_Application->getDriver()->getDriverType();

		// create the imgui interface
		KLS_CHECK(initializeImGui(), "ImGui initialization failed", KLS_FAILURE);
		m_ImGuiContext = ImGui::GetCurrentContext();

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_ImGui::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_ImGui::cleanup()");

		// based on the driver type
		switch (m_DriverType)
		{
		case KLSDT_OPENGL:
		{
			ImGui_ImplOpenGL3_Shutdown();
			ImGui_ImplGlfw_Shutdown();
		} break;
		}

		// destroy the imgui interface
		ImGui::DestroyContext();

		// forget this
		m_Application = nullptr;

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	void KLS_ImGui::beginFrame()
	{
		switch (m_DriverType)
		{
		case KLSDT_OPENGL:
		{
			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();		
		}
		}
	}

	void KLS_ImGui::setCurrentContext()
	{
		ImGui::SetCurrentContext(m_ImGuiContext);
	}

	void KLS_ImGui::endFrame()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	// internal initialization method
	bool KLS_ImGui::initializeImGui()
	{
		getApplication()->getWindow()->makeCurrentContext();

		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsLight();

		switch (m_DriverType)
		{
		case KLSDT_OPENGL:
		{
			ImGui_ImplGlfw_InitForOpenGL(m_Application->getWindow()->getWindow(), true);
			ImGui_ImplOpenGL3_Init("#version 130");
		} break;
		}

		// everything went fine
		return KLS_SUCCESS;
	}

} // end namespace