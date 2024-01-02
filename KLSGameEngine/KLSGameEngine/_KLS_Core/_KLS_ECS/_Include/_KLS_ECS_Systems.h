// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_ECS_System.h"

// unclutter the global namespace
namespace KLS
{
	/*
	*  define all component systems here to make it easier to include only this header and have access to them all
	*/

	// forward declarations
	typedef class KLS_ECS KLS_ECS;

	// controller
	class KLS_API KLS_ECS_System_Camera : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Camera(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// calcualte the distance to the camera for culling / sorting
	class KLS_API KLS_ECS_System_CameraDistance : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_CameraDistance(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// render all of the meshes
	class KLS_API KLS_ECS_System_Mesh : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Mesh(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// render all of the skyboxes
	class KLS_API KLS_ECS_System_Skybox : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Skybox(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// update all of the light positions
	class KLS_API KLS_ECS_System_Lights : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Lights(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// physx
	class KLS_API KLS_ECS_System_PhysX : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_PhysX(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// rotate 
	class KLS_API KLS_ECS_System_Rotate : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Rotate(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// flycircle
	class KLS_API KLS_ECS_System_FlyCircle : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_FlyCircle(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// controller
	class KLS_API KLS_ECS_System_Controller : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Controller(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// deathtimer
	class KLS_API KLS_ECS_System_DeathTimer : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_DeathTimer(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// editor hilite
	class KLS_API KLS_ECS_System_Hilite : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Hilite(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

	// editor hilite
	class KLS_API KLS_ECS_System_Debug : public KLS_ECS_System
	{
	public:
		KLS_ECS_System_Debug(KLS_ECS* ecs);
		virtual void tick(const double& elapsedtime);
	};

} // end namespace