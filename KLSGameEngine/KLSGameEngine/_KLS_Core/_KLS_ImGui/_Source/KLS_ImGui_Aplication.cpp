// incldue the needed header files
#include "KLS_ImGui.h"
#include "KLS_Application.h"
#include "KLS_Window.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h> 
#include "KLS_Timer.h"
#include "_KLS_Driver.h"
#include "KLS_ResourceManager.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"

// unclutter the global namespace
namespace KLS
{
	void KLS_ImGui::RenderAppStats()
	{
		KLS_Timer* timer = m_Application->getTimer();
		ImGui::Begin("Application Statistics");

			ImVec4 modecolor;
			switch (m_Application->getMode())
			{
			case KLS_MODE_INIT	: modecolor = ImVec4(KLS_YELLOW.r, KLS_YELLOW.g, KLS_YELLOW.b, KLS_YELLOW.a); break;
			case KLS_MODE_PAUSE	: modecolor = ImVec4(KLS_RED.r, KLS_RED.g, KLS_RED.b, KLS_RED.a); break;
			case KLS_MODE_RUN	: modecolor = ImVec4(KLS_GREEN.r, KLS_GREEN.g, KLS_GREEN.b, KLS_GREEN.a); break;
			}
			ImGui::TextColored(modecolor, KLS_MODE_TEXT[m_Application->getMode()].c_str());

			ImGui::Text("Total Time ");
				ImGui::SameLine(); 
				ImGui::Text(timer->getTotalTimeString().c_str());
					ImGui::SameLine();
					ImGui::Text("Delta Time ");
					ImGui::SameLine();
						ImGui::Text(timer->getDeltaTimeString().c_str());
						ImGui::SameLine();
							ImGui::Text("FPS ");
							ImGui::SameLine();
							ImGui::Text(timer->getFrameRateString().c_str());

			glm::vec3 cameraPos = getApplication()->getDriver()->getCameraPosition();
			ImGui::Text("Camera ");
							ImGui::SameLine();
							ImGui::Text(std::to_string(cameraPos.x).c_str());
							ImGui::SameLine();
							ImGui::Text(" ");
							ImGui::SameLine();
							ImGui::Text(std::to_string(cameraPos.y).c_str());
							ImGui::SameLine();
							ImGui::Text(" ");
							ImGui::SameLine();
							ImGui::Text(std::to_string(cameraPos.z).c_str());

				glm::vec4 color = m_Application->getDriver()->getClearColor();
				if (ImGui::ColorEdit4("Clear Color", glm::value_ptr(color)))
						m_Application->getDriver()->setClearColor(color);

		ImGui::End();
	}

	void KLS_ImGui::RenderFrameBuffers()
	{
		ImGui::Checkbox("GPU Protect", &m_Application->getDriver()->m_GPUProtect);
		if (m_Application->getDriver()->m_GPUProtect)
		{
			ImGui::SameLine();
			ImGui::InputInt("Max Framerate", &m_Application->getDriver()->m_GPUMaxFrameRate);
		}

		ImGui::Checkbox("Use FBO", &m_Application->getDriver()->m_UseFBO);
		ImGui::Checkbox("Use PostProcessing (in current order)", &m_Application->getDriver()->m_UsePostProcessing);
		if (m_Application->getDriver()->m_UsePostProcessing)
		{
			std::vector<KLS_MATERIAL_TYPE>& l = m_Application->getDriver()->getResourceManager()->getAvailablePostProcesssingMaterialTypes();
			if (!l.empty())
			{
				struct MaterialCheckBox {
					KLS_MATERIAL_TYPE type;
					bool exists;
				};

				// Create a vector of MaterialCheckBox structs
				std::vector<MaterialCheckBox> checkboxes;

				// Populate the vector
				for (int x = 0; x < l.size(); x++) {
					MaterialCheckBox checkBox;
					checkBox.type = l[x];
					checkBox.exists = m_Application->getDriver()->getFrameBufferManager()->postProcessExists(l[x]);
					checkboxes.push_back(checkBox);
				}

				// Sort the checkboxes vector based on existence
				std::sort(checkboxes.begin(), checkboxes.end(), [](const MaterialCheckBox& a, const MaterialCheckBox& b) {
					// Sort by existence (true first)
					return a.exists > b.exists;
					});

				// Loop through the sorted checkboxes to render
				for (const auto& checkBox : checkboxes) 
				{
					bool use = checkBox.exists;
					KLS_MATERIAL_TYPE materialType = checkBox.type;
					ImGui::InvisibleButton("##indent", ImVec2(10, 1));
					ImGui::SameLine();
					if (ImGui::Checkbox(KLS_MATERIAL_TYPE_TEXT[materialType].c_str(), &use))
						m_Application->getDriver()->getFrameBufferManager()->toggleEffect(materialType);
				}
			}
		}
			
#if(0)
		if (m_Application->getDriver()->getUseFBO())
		{
			ImVec2 pos = { 100, 100 };
			ImGui::Image(m_Application->getDriver()->getResourceManager()->getTextureId("fbo0_color"), pos);
			ImGui::Image(m_Application->getDriver()->getResourceManager()->getTextureId("fbo1_color"), pos);
		}
#endif
	}

} // end namespace