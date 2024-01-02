// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_EventHandler.h"
#include "KLS_Transform.h"
#include "KLS_Mesh.h"
#include "KLS_UniformData.h"
#include "KLS_Message.h"
#include <yaml-cpp/yaml.h>
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Application KLS_Application;
	typedef class KLS_Driver KLS_Driver;
	typedef class KLS_ECS KLS_ECS;
	typedef class KLS_Entity KLS_Entity;
	typedef class KLS_PhysX_World KLS_PhysX_World;
	typedef class KLS_PhysX_Object KLS_PhysX_Object;

	class KLS_API KLS_RenderMesh
	{
	public:
		KLS_Mesh* m_Mesh;
		KLS_Transform m_Transform;
		float m_CameraDistance;
	public:
		KLS_RenderMesh(KLS_Mesh* mesh, KLS_Transform t, float cameradistance) : m_Mesh(mesh), m_Transform(t),m_CameraDistance(cameradistance) {}
	};

	class KLS_API KLS_RenderLight
	{
	public:
		KLS_LightData m_LightData;
	public:
		KLS_RenderLight(KLS_LightData data) : m_LightData(data) {}
	};

	/*
	*
	*/
	class KLS_API KLS_Level : public KLS_EventHandler
	{
	private:
		KLS_VARIABLE_GET(KLS_Application*, Application, nullptr);
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_GET(KLS_ECS*, ECS, nullptr);
		KLS_VARIABLE_GET(KLS_PhysX_World*, PhysXWorld, nullptr);		// physx world
		KLS_VARIABLE_GET(int, Id, 0);
		KLS_VARIABLE_EDITABLE_SETGET(float, CullDistance, 1000.0f);

	private:
		void drawSkyboxes();
		void drawSolids();
		void drawTransparent();

	public:
		std::vector<KLS_RenderLight> m_Lights;
		std::vector<KLS_RenderMesh> m_Solid;
		std::vector<KLS_RenderMesh> m_Skyboxes;
		std::vector<KLS_RenderMesh> m_Transparent;

	public:
		KLS_UUID getUniqueId() { return KLS_UUID(); }

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_Level();
		virtual ~KLS_Level();

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

		// update the driver lights
		virtual void updateLightUniforms();

		// event overrides to provide functionality
		virtual bool onSystemFrameBufferResized(const KLS_Event& e);

		// imgui related methods
		virtual void ImGui_draw();

		// select an object using pixel perfect selection
		KLS_Entity getSelectedObject(int mousex, int mousey);

		// handle messages
		virtual void onMessage(const KLS_Message& msg);

		virtual void clear();
		virtual bool saveToDisk(std::string filepath);
		virtual bool loadFromDisk(std::string filepath);
	private:
		void serializeEntity(YAML::Emitter& out, KLS_Entity entity);
		void deserializeEntity(YAML::Node& data, KLS_Entity entity);

	public:
		KLS_Entity createSkybox(std::string skyboxtexturename, std::string texturepath);
		KLS_Entity createGrid();
		KLS_Entity createCube(KLS_Entity parent, KLS_UUID id, KLS_Transform t, bool physx, bool dynamic, glm::vec3 dir, float velocity);
		KLS_Entity createMesh(KLS_Entity parent, KLS_UUID id, KLS_Transform t, std::string meshfilename, KLS_MATERIAL_TYPE mt, bool physx);
		KLS_Entity createLight(KLS_Entity parent, KLS_UUID id, KLS_Transform t, glm::vec4 color);

		void createStackOfCubes(glm::vec3 pos, glm::vec3 scale, int count, bool physics, bool dynamic);
		void createPyramidOfCubes(glm::vec3 pos, int baseXCount, int baseZCount, float cubeSize, bool physics, bool dynamic);

		void rescalePhysxObject(KLS_Entity e);
	};

} // end namespace
