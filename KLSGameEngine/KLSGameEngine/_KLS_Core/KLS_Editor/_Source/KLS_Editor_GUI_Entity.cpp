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
#include "KLS_PhysX_World.h"
#include "KLS_PhysX_Object.h"

#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace KLS
{

    void KLS_Editor::drawEntityList()
    {
        ImGui::Begin("Entities");

        KLS_ECS* ecs = getECS();
        auto view = getECS()->getRegistry().view<KLS_COMPONENT_NAME>(entt::exclude<KLS_COMPONENT_PARENT>);

        // Create a vector to store entities from the view
        std::vector<entt::entity> entities(view.begin(), view.end());

        // Create a lambda function to retrieve KLS_COMPONENT_ID from entities
        auto getId = [&](const entt::entity& entity) { return KLS_Entity(ecs, entity).getId(); };

        // Sort the entities vector based on KLS_COMPONENT_ID property using the getId lambda
        std::sort(entities.begin(), entities.end(), [&](const entt::entity& a, const entt::entity& b) {
            return getId(a) < getId(b);
            });

        int spacer = 0;
        for (auto entity : entities) {
            KLS_Entity e(ecs, entity);
            drawEntityTree(e);
        }
        ImGui::End();
    }

    void KLS_Editor::drawEntityTree(KLS_Entity e)
    {
        KLS_COMPONENT_ID id = e.getComponent<KLS_COMPONENT_ID>();
        KLS_COMPONENT_NAME name = e.getComponent<KLS_COMPONENT_NAME>();
        KLS_COMPONENT_TRANSFORM t = e.getComponent<KLS_COMPONENT_TRANSFORM>();

        // Display the entity in a TreeNode
        bool selected = (id.Id == m_Selected.getId());
        if (selected)  ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 0.0f, 1.0f));
        //        bool nodeOpen = ImGui::TreeNode((void*)(intptr_t)id.Id, "ID: %d Name: %s %f", id.Id, name.m_Name.c_str(), t.m_Transform.getScale().x);
        bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)id.Id, ImGuiTreeNodeFlags_DefaultOpen, "ID: %d Name: %s %f", id.Id, name.m_Name.c_str(), t.m_Transform.getScale().x);

        if (ImGui::IsItemClicked()) setSelected(id.Id,0);
        if (selected)  ImGui::PopStyleColor();

        if (nodeOpen) {
            std::vector<entt::entity> children = e.getChildren();
            for (int x = 0; x < children.size(); x++) {
                KLS_Entity child(getECS(), children[x]);
                drawEntityTree(child);
            }
            ImGui::TreePop(); // Close the TreeNode
        }
    }

    void KLS_Editor::drawEntityEdit()
    {
        if (m_Selected.isNull()) return;

        ImGui::Begin("Entity Edit");

        KLS_ImGui* gui = getApplication()->getImGui();

        std::string name = m_Selected.getName();

        std::string idtext = "ID - ";
        idtext += std::to_string(m_Selected.getId());
        ImGui::TextDisabled(idtext.c_str());

        bool positionchanged = gui->drawVec3Control("Position", m_Selected.getTransform().m_Position, m_Origin.m_Position, 100.0f);

        glm::vec3 rot = glm::eulerAngles(m_Selected.getTransform().m_Rotation);
        glm::vec3 originrot = glm::eulerAngles(m_Origin.m_Rotation);
        bool rotationchanged = gui->drawVec3Control("Rotation", rot, originrot, 100.0f);
        m_Selected.getTransform().m_Rotation = glm::quat(rot);

        bool scalechanged = gui->drawVec3Control("Scale", m_Selected.getTransform().m_Scale, m_Origin.m_Scale, 100.0f);

        // if the entity has a physx component then adjust it
        if (m_Selected.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
        {
            auto& po = m_Selected.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
            if (po.m_PhysxObject)
            {
                if (scalechanged)
                {
                    rescalePhysxObject(m_Selected);
                }
                else
                if (positionchanged)
                {
                    po.m_PhysxObject->setPosition(m_Selected.getTransform().getPosition());
                }
                else
                if (rotationchanged)
                {
                   po.m_PhysxObject->setRotation(m_Selected.getTransform().getRotation());
                }
            }
        }

        ImGui::End();
    }

} // end namespace