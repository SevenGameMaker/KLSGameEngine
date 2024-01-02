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
#include "KLS_Level.h"
#include "KLS_ECS.h"
#include "_KLS_ECS_Components.h"
#include "KLS_Camera.h"

// unclutter the global namespace
namespace KLS
{
	void KLS_ImGui::RenderLevelInfo(KLS_Level* level)
	{
		ImGui::Begin("Level Info");

		int count, inuse; 
		level->getECS()->getCount(count, inuse); 
		std::string t = "Entity count : "; t += std::to_string(count); t += " in use : "; t += std::to_string(inuse);
		t+= " Light count : "; t += std::to_string(level->getDriver()->getNumberOfLights());
		ImGui::Text(t.c_str());
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("Camera Near", &level->getDriver()->getActiveCamera()->m_NearPlane); ImGui::SameLine();
		ImGui::SliderFloat("*1", &level->getDriver()->getActiveCamera()->m_NearPlane, 0.01f, level->getDriver()->getActiveCamera()->m_FarPlane, "", 10);
		ImGui::SetNextItemWidth(60);
		ImGui::InputFloat("Camera Far", &level->getDriver()->getActiveCamera()->m_FarPlane); ImGui::SameLine();
		ImGui::SliderFloat("*2", &level->getDriver()->getActiveCamera()->m_FarPlane, 0.01f, 1000.0f, "", 10);

		RenderLevelEntities(level);

		ImGui::End();
	}

	void KLS_ImGui::RenderLevelEntities(KLS_Level* level)
	{
		KLS_ECS* ecs = level->getECS();
		auto view = level->getECS()->getRegistry().view<KLS_COMPONENT_NAME>(entt::exclude<KLS_COMPONENT_PARENT>);

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
			RenderEntityTree(level, e);
		}
	}

	void KLS_ImGui::RenderEntity(KLS_Level* level, int& spacer, KLS_Entity e)
	{
		int oldspacer = spacer;

		KLS_COMPONENT_ID	id = e.getComponent<KLS_COMPONENT_ID>();
		KLS_COMPONENT_NAME	name = e.getComponent<KLS_COMPONENT_NAME>();
		KLS_COMPONENT_TRANSFORM	t = e.getComponent<KLS_COMPONENT_TRANSFORM>();
		std::string spaces;
		padToLength(spaces, spacer, ' ');
		ImGui::Text("%s ID: %d Name: %s %f", spaces.c_str(), id.Id, name.m_Name.c_str(),t.m_Transform.getScale().x);

		spacer++;
		std::vector<entt::entity> children = e.getChildren();
		if (!children.empty())
		{
			for (int x = 0; x < children.size(); x++)
			{
				KLS_Entity child(level->getECS(), children[x]);
				RenderEntity(level, spacer, child);
			}
		}
		spacer = oldspacer;
	}

	void KLS_ImGui::RenderEntityTree(KLS_Level* level, KLS_Entity e)
	{
		KLS_COMPONENT_ID id = e.getComponent<KLS_COMPONENT_ID>();
		KLS_COMPONENT_NAME name = e.getComponent<KLS_COMPONENT_NAME>();
		KLS_COMPONENT_TRANSFORM t = e.getComponent<KLS_COMPONENT_TRANSFORM>();

		// Display the entity in a TreeNode
		bool nodeOpen = ImGui::TreeNode((void*)(intptr_t)id.Id, "ID: %d Name: %s %f", id.Id, name.m_Name.c_str(), t.m_Transform.getScale().x);

		if (nodeOpen) {
			std::vector<entt::entity> children = e.getChildren();
			for (int x = 0; x < children.size(); x++) {
				KLS_Entity child(level->getECS(), children[x]);
				RenderEntityTree(level, child);
			}
			ImGui::TreePop(); // Close the TreeNode
		}
	}
} // end namespace