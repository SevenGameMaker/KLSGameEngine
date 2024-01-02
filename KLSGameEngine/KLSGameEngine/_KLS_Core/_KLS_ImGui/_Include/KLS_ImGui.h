// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

typedef struct ImGuiContext ImGuiContext;

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Application KLS_Application;
	typedef class KLS_Level KLS_Level;
	typedef class KLS_Entity KLS_Entity;

	/*
	*
	*/
	class KLS_API KLS_ImGui
	{
	private:
		KLS_VARIABLE_GET(KLS_Application*, Application, nullptr);
		KLS_VARIABLE_GET(KLS_DRIVER_TYPE, DriverType, KLSDT_NONE);
		KLS_VARIABLE_GET(ImGuiContext*, ImGuiContext, nullptr);			// the ImGui context (needed outside of this dll memory)

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_ImGui();
		virtual ~KLS_ImGui();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// set the current context back to this
		void setCurrentContext();

		// frame methods
		void beginFrame();
		void endFrame();

		// helper methods
		void drawColorEdit4(const std::string& label, glm::vec4& values);
		void drawTextureArray(const std::string& label, std::vector<std::string>& textures, std::string path);
		bool drawVec3Control(const std::string& label, glm::vec3& values, glm::vec3 resetValue, float columnWidth = 100.0f);
		bool drawQuatControl(const std::string& label, glm::quat& value, glm::quat resetValue, float columnWidth = 100.0f);

	private:
		// internal initialization method
		bool initializeImGui();
	};


} // end namespace