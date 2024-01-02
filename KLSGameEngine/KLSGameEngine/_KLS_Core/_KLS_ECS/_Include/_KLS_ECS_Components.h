// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

#include "KLS_Transform.h"
#include "KLS_Mesh.h"
#include "KLS_Camera.h"
#include "KLS_Camera_FPS.h"
#include "KLS_Entity.h"
#include "KLS_UniformData.h"
#include "KLS_PhysX_Object.h"
#include "KLS_UUID.h"

// unclutter the global namespace
namespace KLS
{
	/*
	*  define all component strcutures here to make it easier to include only this header and have access to them all
	*/
	struct KLS_API KLS_COMPONENT_ID 
	{
		KLS_UUID Id;
	};

	struct KLS_API KLS_COMPONENT_NAME 
	{
		std::string m_Name;
	};

	struct KLS_API KLS_COMPONENT_TRANSFORM 
	{
		KLS_Transform m_Transform;
	};

	struct KLS_API KLS_COMPONENT_PARENT 
	{
		KLS_Entity m_Parent;
	};

	struct KLS_API KLS_COMPONENT_CHILDREN 
	{
		std::vector<entt::entity> m_Children;
	};

	struct KLS_API KLS_COMPONENT_CAMERA_DISTANCE 
	{
		float m_CameraDistance;
	};

	struct KLS_API KLS_COMPONENT_CAMERA 
	{
		KLS_Camera* Camera;
	};

	struct KLS_API KLS_COMPONENT_MESH 
	{
		KLS_Mesh* m_Mesh;
	};

	struct KLS_API KLS_COMPONENT_SKYBOX 
	{
		KLS_Mesh* m_Mesh;
	};

	struct KLS_API KLS_COMPONENT_LIGHT 
	{
		KLS_LightData m_LightData;
	};

	struct KLS_API KLS_COMPONENT_PHYSXOBJECT 
	{
		KLS_PhysX_Object* m_PhysxObject = nullptr;
	};

	struct KLS_API KLS_COMPONENT_ROTATE 
	{
		glm::vec3 m_RotateSpeed = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_RotationDirection = glm::vec3(0.0f, 1.0f, 0.0f);
		double m_TotalTime = 0.0f;
	};

	struct KLS_API KLS_COMPONENT_FLYCIRCLE 
	{
		glm::vec3 m_Center = glm::vec3(0.0f,0.0f,0.0f);
		float m_Radius =  10.0f;
		float m_Speed = 1.0f;

		glm::vec3 m_Direction = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_VecU = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 m_VecV = glm::vec3(0.0f, 0.0f, 0.0f);
		float m_RadiusEllipsoid = 0.0f;
		float m_StartTime = 0.0f;
		bool m_First = true;
	};

	struct KLS_COMPONENT_CONTROLLER 
	{
		KLS_KeyMap KeyMap;
		bool	MoveForward = false;
		bool	MoveBackward = false;
		bool	MoveTurnLeft = false;
		bool	MoveTurnRight = false;
		bool	MoveStrafeLeft = false;
		bool	MoveStrafeRight = false;
		float	MovementSpeed = 10.0f;
		float	MovementTurnSpeed = 0.1f;
		float	MovementMultipler = 50.0f;

		bool	UseMouse = false;
		float	MouseX = 0;
		float	MouseY = 0;
		float	LastMouseX = 0;
		float	LastMouseY = 0;
		bool	FirstMouse = true;
		float	MouseSensitivity = 2.50f;
		float Pitch = 0;
		float Yaw = 0;
	};

	struct KLS_API KLS_COMPONENT_DEATHTIMER 
	{
		float Timer;
	};

	struct KLS_API KLS_COMPONENT_HILITE
	{
		int MeshBuffer = 0;
	};

	struct KLS_API KLS_COMPONENT_DEBUG
	{
		bool Active;
		KLS_Mesh* m_Mesh = nullptr;
	};

} // end namespace