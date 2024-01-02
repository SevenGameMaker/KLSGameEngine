// incldue the needed ehader files
#include "KLS_Editor.h"
#include "KLS_Application.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"
#include "KLS_Camera.h"
#include "KLS_FrameBufferManager.h"
#include "KLS_FrameBuffer.h"
#include "KLS_ResourceManager.h"
#include "KLS_Texture.h"
#include "KLS_Math.h"
#include "KLS_Timer.h"
#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace KLS
{
    void KLS_Editor::drawLevelInfo()
    {
        ImGui::Begin("Level Info");

        ImGui::Checkbox("Protect GPU", &getDriver()->m_GPUProtect);
        ImGui::SameLine();
        std::string fpstext = "FPS :" + std::to_string(getApplication()->getTimer()->getFPS());
        ImGui::Text(fpstext.c_str());

        int count, inuse;
        getECS()->getCount(count, inuse);
        std::string t = "Entity count : "; t += std::to_string(count); t += " in use : "; t += std::to_string(inuse);
        t += " Light count : "; t += std::to_string(getDriver()->getNumberOfLights());
        ImGui::Text(t.c_str());

        t = "Solid count : "; t += std::to_string(m_Solid.size()); t += " Transparent count : "; t += std::to_string(m_Transparent.size());
        ImGui::Text(t.c_str());

        ImGui::SetNextItemWidth(60);
        ImGui::InputFloat("Camera Near", &m_Camera->m_NearPlane); ImGui::SameLine();
        ImGui::SliderFloat("*1", &m_Camera->m_NearPlane, 0.01f, m_Camera->m_FarPlane, "%.3f", 0);
        ImGui::SetNextItemWidth(60);
        ImGui::InputFloat("Camera Far", &m_Camera->m_FarPlane); ImGui::SameLine();
        ImGui::SliderFloat("*2", &m_Camera->m_FarPlane, 0.01f, 1000.0f, "", 0);
        ImGui::ColorEdit4("Ambient Color", &getDriver()->m_AmbientColor[0]);
        ImGui::SliderFloat("Ambient Intensity", &getDriver()->m_AmbientIntensity, 0.01f, 1.0f, "%.3f", 0);

        ImGui::End();
    }

} // end namespace