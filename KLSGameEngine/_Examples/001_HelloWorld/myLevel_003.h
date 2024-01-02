// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Level.h"
#include "KLS_Entity.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Camera_FPS KLS_Camera_FPS;
	typedef class KLS_Mesh KLS_Mesh;

	/*
	*
	*/
	class myLevel_003 : public KLS_Level
	{
	private:
		KLS_VARIABLE_GET(KLS_Camera_FPS*, Camera, nullptr);
		KLS_VARIABLE_SETGET(KLS_Entity, Selected, KLS_Entity());

	public:
		// class constructor / destructor (use create / cleanup instead)
		myLevel_003();
		virtual ~myLevel_003();

		// dual creation allows for better error handling
		virtual bool create(KLS_Application* app, int id);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// load / unload as level is made active / inactive
		virtual void load();
		virtual void unload();

		// event overrides to provide functionality
		virtual bool onLMousePressed(const KLS_Event& e);
		virtual bool onMMousePressed(const KLS_Event& e);
		virtual bool onRMousePressed(const KLS_Event& e);

		// imgui related methods
		virtual void ImGui_draw();

		virtual void createWorld();
		virtual void destroyWorld();

	};

} // end namespace
