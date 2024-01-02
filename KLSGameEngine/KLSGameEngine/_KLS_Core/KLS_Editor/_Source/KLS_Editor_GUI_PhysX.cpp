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
#include "KLS_File.h"
#include "KLS_String.h"
#include "KLS_ImGui.h"
#include "KLS_Entity.h"
#include "_KLS_ECS_Components.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <KLS_PhysX_ObjectCreationParams.h>
#include "KLS_PhysX_World.h"
#include "KLS_PhysX_Object.h"

namespace KLS
{
    void KLS_Editor::drawPhysXInfo()
    {
        KLS_ImGui* imgui = getApplication()->getImGui();
        if (m_Selected.isNull()) return;

        ImGui::Begin("PhysX");

        KLS_PhysX_ObjectCreationParams params;

        // if there is already a physx object then grab the data
        if (m_Selected.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
        {
            auto& physx = m_Selected.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
            if (physx.m_PhysxObject) params = physx.m_PhysxObject->getParams();

            // if the user want to remove the current physx object then do so
            if (ImGui::Button("Remove"))
            {
                if (physx.m_PhysxObject)
                {
                    physx.m_PhysxObject->cleanup();
                    delete(physx.m_PhysxObject);
                    physx.m_PhysxObject = nullptr;
                    m_Selected.removeComponent<KLS_COMPONENT_PHYSXOBJECT>();
                    ImGui::End();
                    return;
                }
            }
        }

        bool Dirty = false;

        if (ImGui::CollapsingHeader("PhysX Object", 0))
        {
            if (ImGui::TreeNode("Create Info "))
            {
                int currentItem = params.Type;
                if (ImGui::BeginCombo("PhysX Type", KLS_PHYSXTYPE_TEXT[currentItem].c_str()))
                {
                    for (int i = 0; i < POT_COUNT; i++)
                    {
                        const bool isSelected = (currentItem == i);
                        if (ImGui::Selectable(KLS_PHYSXTYPE_TEXT[i].c_str(), isSelected)) currentItem = i;
                        if (isSelected) ImGui::SetItemDefaultFocus();
                        params.Type = (KLS_PHYSX_TYPE)currentItem;
                        Dirty = true;
                    }
                    ImGui::EndCombo();
                }

                if (params.Type != POT_TREE) { if (ImGui::Checkbox("Dynamic", &params.Dynamic)) Dirty = true; }
                else params.Dynamic = false;
                if (ImGui::Checkbox("Trigger", &params.IsTrigger)) Dirty = true;
                if (ImGui::InputFloat("Mass", &params.Mass)) Dirty = true;

                params.Position = m_Selected.getTransform().getPosition();
                params.Rotation = m_Selected.getTransform().getRotation();
                params.Scale = m_Selected.getTransform().getScale();
                params.ObjectId = m_Selected.getId();
                params.ObjectType = KLSOT_OBJECT;

                ImGui::TreePop();
            }
        }

        if (Dirty)
        {
                // if there is already a physx object then delete it
                if (m_Selected.hasComponent<KLS_COMPONENT_PHYSXOBJECT>())
                {
                    auto& physx = m_Selected.getComponent<KLS_COMPONENT_PHYSXOBJECT>();
                    if (physx.m_PhysxObject)
                    {
                        physx.m_PhysxObject->cleanup();
                        delete(physx.m_PhysxObject);
                        physx.m_PhysxObject = nullptr;
                        m_Selected.removeComponent<KLS_COMPONENT_PHYSXOBJECT>();
                    }
                }

                // if there is already a physx object then grab the data
                if (m_Selected.hasComponent<KLS_COMPONENT_MESH>())
                {
                    auto& mesh = m_Selected.getComponent<KLS_COMPONENT_MESH>();
                    params.Mesh = mesh.m_Mesh;
                }

                KLS_COMPONENT_PHYSXOBJECT po;
                   po.m_PhysxObject = getPhysXWorld()->createPhysXObject(params);
                   if (po.m_PhysxObject) m_Selected.addComponent<KLS_COMPONENT_PHYSXOBJECT>(po);
        }

        ImGui::End();
    }

} // end namespace