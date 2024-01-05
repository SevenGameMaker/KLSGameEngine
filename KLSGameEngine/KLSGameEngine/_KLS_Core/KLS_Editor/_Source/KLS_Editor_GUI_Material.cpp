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

namespace KLS
{
    void KLS_Editor::drawMaterialSelector()
    {
        KLS_ImGui* imgui = getApplication()->getImGui();

        if (m_Selected.isNull()) return;
        if (!m_Selected.hasComponent<KLS_COMPONENT_MESH>()) return;

        auto& mesh = m_Selected.getComponent<KLS_COMPONENT_MESH>();
        
        ImGui::Begin("Material");

        if (ImGui::CollapsingHeader("Mesh", 0))
        {
           if (ImGui::TreeNode("Mesh Filename "))
           {
               std::string current = "";
               KLS_Material mat;
               if (mesh.Mesh)
               {
                  current = mesh.Mesh->getName();
                  mat = mesh.Mesh->getMaterial(0);
               }

               if (ImGui::Button("file"))
               {
                   std::string filename = getFile(current, KLS_MESHPATH, KLS_FILES_MESH);
                   if (filename != current)
                   {
                       getDriver()->getResourceManager()->removeMesh(mesh.Mesh);
                       mesh.Mesh = getDriver()->getResourceManager()->getMesh(filename);
                       if (mesh.Mesh) mesh.Mesh->getMaterial(0).MaterialType = mat.MaterialType;
                       if (m_Selected.hasComponent<KLS_COMPONENT_HILITE>())
                           m_Selected.removeComponent<KLS_COMPONENT_HILITE>();
                   }
               }
               ImGui::SameLine();
               ImGui::Text(current.c_str());

               ImGui::TreePop();
            }
        }

        for (int x = 0; x < mesh.Mesh->getMaterialCount(); x++)
        {
            KLS_Material& mat = mesh.Mesh->getMaterial(x);

            std::string title = "Material" + std::to_string(x);
            ImGui::PushID(title.c_str());

            ImGuiTreeNodeFlags flags = (x == m_SelectedMeshBuffer) ? ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Selected : 0;
            if (ImGui::CollapsingHeader(title.c_str(), flags))
            {
                int currentItem = mat.MaterialType;
                if (ImGui::BeginCombo("Material Type", KLS_MATERIAL_TYPE_TEXT[currentItem].c_str())) 
                {
                    for (int i = 0; i < KLS_MT_COUNT; i++) 
                    {
                        const bool isSelected = (currentItem == i);
                        
                        if (ImGui::Selectable(KLS_MATERIAL_TYPE_TEXT[i].c_str(), isSelected))
                            currentItem = i;
                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                        mat.MaterialType = currentItem;
                    }
                    ImGui::EndCombo();
                }

                ImGui::Separator();
                imgui->drawColorEdit4("Diffuse", mat.ColorDiffuse);
                imgui->drawColorEdit4("Ambient", mat.ColorAmbient);
                imgui->drawColorEdit4("Specular", mat.ColorSpecular);
                imgui->drawColorEdit4("Emmissive", mat.ColorEmmissive);
                ImGui::SliderFloat("Shininess", &mat.Shininess, 0.0f, 1.0f, "%.3f", 0);
                ImGui::Checkbox("Transparent", &mat.Transparency);
                ImGui::Checkbox("Wireframe", &mat.Wireframe);
                ImGui::SliderFloat("Mix", &mat.Mix, 0.0f, 1.0f, "%.3f", 0);

                ImGui::Separator();
                imgui->drawTextureArray("Diffuse Textures", mat.Texture_Diffuse, KLS_TEXTUREPATH);
                imgui->drawTextureArray("Specular Textures", mat.Texture_Specular, KLS_TEXTUREPATH);
                imgui->drawTextureArray("Normal Textures", mat.Texture_Normal, KLS_TEXTUREPATH);
                imgui->drawTextureArray("Displacement Textures", mat.Texture_Displacement, KLS_TEXTUREPATH);
            }
            ImGui::PopID();
        }
        ImGui::End();
    }


} // end namespace