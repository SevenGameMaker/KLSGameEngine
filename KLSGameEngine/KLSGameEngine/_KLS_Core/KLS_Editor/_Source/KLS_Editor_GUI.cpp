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
    void KLS_Editor::drawMainWindow()
    {
        ImGui::DockSpaceOverViewport();

        bool open = true;
        ImGui::Begin("Editor", &open, ImGuiWindowFlags_MenuBar);
        drawMenu();
        ImGui::DockSpace(maindock);
        ImGui::End();

        drawContentWindow();
        drawToolbar();
        drawEntityList();
        drawResourceSelector();

        bool infoopen = true;
        ImGui::Begin("Info", &infoopen,0);
        ImGui::DockSpace(infodock);
        ImGui::End();

        drawLevelInfo();
        drawPhysXInfo();
        drawMaterialSelector();
        drawEntityEdit();

    }

} // end namespace