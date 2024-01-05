// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Level.h"
#include "KLS_Entity.h"
#include <imgui.h>
#include <ImGuizmo.h>
#include <KLS_ECS_YAML_Helpers.h>

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Camera_FPS KLS_Camera_FPS;
	typedef class KLS_Mesh KLS_Mesh;

	/*
	*
	*/
	class KLS_API KLS_Editor : public KLS_Level
	{
	private:
		KLS_VARIABLE_GET(KLS_Camera_FPS*, Camera, nullptr);
		KLS_VARIABLE_SETGET(KLS_Entity, Skybox, KLS_Entity());
		KLS_VARIABLE_SETGET(KLS_Entity, Grid, KLS_Entity());
		KLS_VARIABLE_SETGET(KLS_Entity, Light, KLS_Entity());

		KLS_VARIABLE_SETGET(ImGuizmo::OPERATION, Operation, ImGuizmo::OPERATION::TRANSLATE);
		KLS_VARIABLE_SETGET(glm::vec4, WindowPosition, glm::vec4(0));

		KLS_VARIABLE_GET(KLS_Entity, Selected, KLS_Entity());
		KLS_VARIABLE_GET(int, SelectedMeshBuffer, 0);
		KLS_VARIABLE_SETGET(KLS_Transform, Origin, KLS_Transform());

		KLS_VARIABLE_GET(bool, ShowDebugInfo, false);
		KLS_VARIABLE_GET(bool, ShowSelectedBuffer, false);

		int maindock = 1;
		int infodock = 22;
	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_Editor();
		virtual ~KLS_Editor();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app, int id);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// load / unload as level is made active / inactive
		virtual void load();
		virtual void unload();

		// frame methods
		virtual void preFrame(const double& elapsedtime);
		virtual void frame(const double& elapsedtime);
		virtual void render();

		// event overrides to provide functionality
		virtual bool onLMousePressed(const KLS_Event& e);
		virtual bool onMMousePressed(const KLS_Event& e);
		virtual bool onRMousePressed(const KLS_Event& e);

		// event overrides to provide functionality
		virtual bool onKeyDown(const KLS_Event& e);

		virtual void setSelected(KLS_UUID id, int meshbuffer);
		virtual void setShowDebugInfo(bool value);
		virtual void setShowSelectedBuffer(bool value);

		// imgui related methods
		virtual void ImGui_draw();

		// world creation methods
		virtual void createDefaultWorld();
		virtual void destroyWorld();

		void toggleGrid();
		void toggleSkybox();



		void drawMainWindow();
		void drawMenu();

			void drawContentWindow();
				void drawToolbar();
				void drawGizmo(ImVec2 dims);
			void drawEntityList();
			void drawEntityEdit();
			void drawEntityTree(KLS_Entity e);
			void drawResourceSelector();
			void drawLevelInfo();
			void drawMaterialSelector();
			void drawPhysXInfo();
	};

} // end namespace
