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
    void KLS_Editor::drawMenu()
    {
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Open Project...", "Ctrl+O"))
				{
					loadFromDisk("D:/_Programming/_Best Core Application/KLS Game Engine Core/_Examples/_Bin/_SaveGames/testing.cs");
				}

				ImGui::Separator();

				if (ImGui::MenuItem("New Scene", "Ctrl+N"))
				{
					setSelected(-1,0);
					clear();
					m_Grid = createGrid();
					getCamera()->setPosition(glm::vec3(-2, 2, -2));
					getCamera()->setTarget(glm::vec3(0, 0, 0));
				}

				if (ImGui::MenuItem("Save Scene", "Ctrl+S"))
				{
					saveToDisk("D:/_Programming/_Best Core Application/KLS Game Engine Core/_Examples/_Bin/_SaveGames/testing.cs");
				}

				if (ImGui::MenuItem("Save Scene As...", "Ctrl+Shift+S"))
				{

				}

				ImGui::Separator();

				if (ImGui::MenuItem("Exit"))
				{
					getApplication()->setMode(KLS_MODE_QUIT);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				if (ImGui::MenuItem("Skybox", "Ctrl+O"))
				{
					toggleSkybox();
				}

				ImGui::Separator();

				if (ImGui::MenuItem("Grid", "Ctrl+S"))
				{
					toggleGrid();
				}

				ImGui::Separator();
				if (ImGui::MenuItem("DebugInfo", "Ctrl+S"))
				{
					setShowDebugInfo(!getShowDebugInfo());
				}

				ImGui::Separator();
				if (ImGui::MenuItem("Meshbuffer", "Ctrl+S"))
				{
					setShowSelectedBuffer(!getShowSelectedBuffer());
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}


} // end namespace