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

#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace KLS
{
    void KLS_Editor::drawToolbar()
    {
        KLS_ResourceManager* rm = getDriver()->getResourceManager();
        std::string path = "../../../../_media/_assets/_textures/_gui/";
        ImVec2 bdims = { 16, 16 };
        ImGui::Begin("Toolbar");
        if (ImGui::ImageButton(rm->getTextureId(path + "Translate.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { m_Operation = ImGuizmo::TRANSLATE; };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Translate"); ImGui::SameLine();
        if (ImGui::ImageButton(rm->getTextureId(path + "Rotate.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { m_Operation = ImGuizmo::ROTATE; };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Rotate"); ImGui::SameLine();
        if (ImGui::ImageButton(rm->getTextureId(path + "Scale.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { m_Operation = ImGuizmo::SCALE; };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Scale"); ImGui::SameLine();
        if (ImGui::ImageButton(rm->getTextureId(path + "Universal.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { m_Operation = ImGuizmo::UNIVERSAL; };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Universal"); ImGui::SameLine();
        if (ImGui::ImageButton(rm->getTextureId(path + "Grid.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { toggleGrid(); };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Grid"); ImGui::SameLine();
        if (ImGui::ImageButton(rm->getTextureId(path + "Skybox.png"), bdims, ImVec2(0, 1), ImVec2(1, 0))) { toggleSkybox(); };
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Skybox"); ImGui::SameLine();

        ImGui::End();
    }

    void KLS_Editor::drawContentWindow()
    {
        ImGui::Begin("MainWindow1");
        ImVec2 dims = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImGui::GetWindowPos();
        m_WindowPosition = glm::vec4(pos.x, pos.y, dims.x, dims.y);
        getDriver()->getFrameBufferManager()->getFrameBuffer("imguifbo")->resize(static_cast<int>(dims.x), static_cast<int>(dims.y));
        getDriver()->setViewport(static_cast<int>(m_WindowPosition.z), static_cast<int>(m_WindowPosition.w));
        ImTextureID id = getDriver()->getResourceManager()->getTextureId("imguifbo_color");
        ImGui::Image(id, dims, ImVec2(0, 1), ImVec2(1, 0));

        ImVec2 imagedims = ImGui::GetContentRegionMax();
        ImVec2 imagepos = ImGui::GetWindowPos();

        // hmmmmmmmmm ??????????
        imagepos.x += 7;
        imagepos.y += 24;
        m_WindowPosition = glm::vec4(imagepos.x, imagepos.y, imagepos.x + imagedims.x, imagepos.y + imagedims.y);

        drawGizmo(ImVec2(0, 0));
        ImGui::End();
    }

    void KLS_Editor::drawGizmo(ImVec2 dims)
    {
        if (!m_Selected.isNull())
        {
            // remember where the entity currently is
            auto& transform = m_Selected.getComponent<KLS_COMPONENT_TRANSFORM>().m_Transform;
            glm::mat4 mat = m_Selected.getAbsoluteTransform().getModelMatrix();
            glm::vec3 lastpos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]);
            glm::vec3 lastscale = glm::vec3(glm::length(glm::vec3(mat[0])), glm::length(glm::vec3(mat[1])), glm::length(glm::vec3(mat[2])));
            glm::quat lastrot = transform.getRotation();

            // use last window position and size
            ImVec2 p = ImGui::GetWindowPos();
            ImVec2 d = ImGui::GetWindowSize();
            glm::mat4 view = getDriver()->getActiveCamera()->getViewMatrix();
            glm::mat4 proj = getDriver()->getActiveCamera()->getProjMatrix(getDriver()->getWindow()->getRatio());
            ImGuizmo::SetRect(p.x, p.y, d.x, d.y);

            ImGuizmo::SetDrawlist();
            ImGuizmo::Manipulate(glm::value_ptr(view), glm::value_ptr(proj), m_Operation, ImGuizmo::LOCAL, glm::value_ptr(mat));

            bool positionchanged = false;
            bool rotationchanged = false;
            bool scalechanged = false;
            if (ImGuizmo::IsUsing())
            {
                switch (m_Operation)
                {
                case ImGuizmo::OPERATION::UNIVERSAL:
                {
                    glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]) - lastpos + transform.getPosition();
                    transform.setPosition(pos);
                    positionchanged = true;

                    glm::vec3 scale = glm::vec3(glm::length(glm::vec3(mat[0])), glm::length(glm::vec3(mat[1])), glm::length(glm::vec3(mat[2])))
                        - lastscale + transform.getScale();
                    if (scale.x < 0.1f) scale.x = 0.1f;
                    if (scale.y < 0.1f) scale.y = 0.1f;
                    if (scale.z < 0.1f) scale.z = 0.1f;

                    if (scale != lastscale)
                    {
                        transform.setScale(scale);
                        scalechanged = true;
                    }
                } break;
                case ImGuizmo::OPERATION::TRANSLATE:
                {
                    glm::vec3 pos = glm::vec3(mat[3][0], mat[3][1], mat[3][2]) - lastpos + transform.getPosition();
                    transform.setPosition(pos);
                    positionchanged = true;
                }  break;
                case ImGuizmo::OPERATION::SCALE:
                {
                    glm::vec3 scale = glm::vec3(glm::length(glm::vec3(mat[0])), glm::length(glm::vec3(mat[1])), glm::length(glm::vec3(mat[2])))
                        - lastscale + transform.getScale();
                    if (scale.x < 0.1f) scale.x = 0.1f;
                    if (scale.y < 0.1f) scale.y = 0.1f;
                    if (scale.z < 0.1f) scale.z = 0.1f;
                    transform.setScale(scale);
                    scalechanged = true;
                }  break;
                case ImGuizmo::OPERATION::ROTATE:
                {
                    glm::vec3 t, r, s;
                    ImGuizmo::DecomposeMatrixToComponents(value_ptr(mat), value_ptr(t), value_ptr(r), value_ptr(s));
                    r = glm::radians(r);
                    m_Selected.getTransform().setRotation(glm::quat(r));
                    rotationchanged = true;
                } break;
                }
            }

            // if the entity has a physx component then adjust it
            if (m_Selected.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
            {
                auto& po = m_Selected.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
                if (po.m_PhysxObject)
                {
                    if (scalechanged)
                    {
                        rescalePhysxObject(m_Selected);
                        if (m_Selected.hasComponent<KLS_COMPONENT_MESH>())
                        {
                            auto& mesh = m_Selected.getComponent< KLS_COMPONENT_MESH>();
                            if (mesh.m_Mesh)
                                mesh.m_Mesh->recalculateNormals();
                        }
                    }
                    if (positionchanged)
                    {
                        po.m_PhysxObject->setPosition(m_Selected.getTransform().getPosition());
                    }
                    if (rotationchanged)
                    {
                        po.m_PhysxObject->setRotation(m_Selected.getTransform().getRotation());
                    }
                }
            }
        }
    }

} // end namespace