// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Mesh.h"
#include "KLS_UUID.h"

// unclutter the global anmespace
namespace KLS
{
	struct KLS_PhysX_ObjectCreationParams
	{
		KLS_PhysX_ObjectCreationParams() {}
		KLS_PhysX_ObjectCreationParams(
			KLS_PHYSX_TYPE type,
			glm::vec3 position,
			glm::quat rotation,
			glm::vec3 scale,
			float mass,
			bool dynamic,
			int material,
			float angularDamping,
			float linearDamping,
			float linearVelocity,
			bool istrigger,
			KLS_OBJECT_TYPE objecttype,
			KLS_UUID objectid,
			KLS_Mesh* mesh
		) :
			Type(type),
			Position(position),
			Rotation(rotation),
			Scale(scale),
			Mass(mass),
			Dynamic(dynamic),
			Material(material),
			AngularDamping(angularDamping),
			LinearDamping(linearDamping),
			LinearVelocity(linearVelocity),
			IsTrigger(istrigger),
			ObjectType(objecttype),
			ObjectId(objectid),
			Mesh(mesh)
		{};

		KLS_PHYSX_TYPE Type = POT_CUBE;
		glm::vec3 Position { 0, 0, 0 };
		glm::quat Rotation { 1, 0, 0, 0 };
		glm::vec3 Scale { 0, 0, 0 };
		float Mass{ 10 };
		bool Dynamic{ true };
		float AngularDamping{ KLS_ANGULAR_DAMPENING };
		float LinearDamping{ KLS_LINEAR_DAMPENING };
		float LinearVelocity{ 0.0f };
		int Material{ 0 };
		bool IsTrigger{ false };
		KLS_OBJECT_TYPE ObjectType{ KLSOT_OBJECT };
		KLS_UUID ObjectId;
		KLS_Mesh* Mesh = nullptr;
	};

} // end namespace
