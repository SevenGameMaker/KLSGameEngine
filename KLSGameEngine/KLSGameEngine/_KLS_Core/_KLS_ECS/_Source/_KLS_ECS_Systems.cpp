// include the default header files
#include "_KLS_ECS_Systems.h"
#include "KLS_Logger.h"
#include "KLS_ECS.h"
#include "_KLS_ECS_Components.h"
#include "KLS_Level.h"
#include "KLS_Application.h"
#include "KLS_ResourceManager.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"
#include "KLS_Entity.h"
#include "KLS_Geometry.h"

// unclutter the global namespace
namespace KLS
{
	// keyboard controller
	KLS_ECS_System_Camera::KLS_ECS_System_Camera(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Camera::tick(const double& elapsedtime)
	{
		KLS_Driver* driver = getECS()->getLevel()->getDriver();

		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_CONTROLLER>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			KLS_COMPONENT_CAMERA& c = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA>(entity);
			c.Camera->onAnimate(elapsedtime);
			t.m_Transform = c.Camera->getTransform();
		}
	};


	// update the disance to the camera for all entities
	KLS_ECS_System_CameraDistance::KLS_ECS_System_CameraDistance(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_CameraDistance::tick(const double& elapsedtime)
	{
		glm::vec3 camPosition = getECS()->getLevel()->getDriver()->getCameraPosition();
		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_CAMERA_DISTANCE>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			KLS_COMPONENT_CAMERA_DISTANCE& cd = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA_DISTANCE>(entity);

			KLS_Entity temp(getECS(), entity);
			KLS_Transform transform = temp.getAbsoluteTransform();
			glm::vec3 pos = transform.getPosition();

			cd.m_CameraDistance = glm::distance(camPosition, pos);
		}
	}

	// light position update
	KLS_ECS_System_Lights::KLS_ECS_System_Lights(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Lights::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_LIGHT>();
		for (entt::entity entity : view)
		{
			KLS_Entity l(getECS(), entity);
			KLS_Transform t = l.getAbsoluteTransform();
			auto& lc = l.getComponent<KLS_COMPONENT_LIGHT>();
			KLS_COMPONENT_CAMERA_DISTANCE& cd = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA_DISTANCE>(entity);
			lc.m_LightData.position = glm::vec4(t.getPosition(), 1.0f);
			lc.m_LightData.cameraDistance = cd.m_CameraDistance;
			getECS()->getLevel()->m_Lights.push_back(KLS_RenderLight(lc.m_LightData));
		}
	}

	// mesh system
	KLS_ECS_System_Mesh::KLS_ECS_System_Mesh(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Mesh::tick(const double& elapsedtime)
	{
		auto view = getECS()->getRegistry().view<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_CAMERA_DISTANCE, KLS_COMPONENT_MESH>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			
			KLS_COMPONENT_MESH& sb = getECS()->getRegistry().get<KLS_COMPONENT_MESH>(entity);
			
			KLS_COMPONENT_CAMERA_DISTANCE& cd = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA_DISTANCE>(entity);

			if ((sb.m_Mesh) && (cd.m_CameraDistance < getECS()->getLevel()->getCullDistance()))
			{
				KLS_Entity temp(getECS(), entity);
				KLS_COMPONENT_ID& id = temp.getComponent<KLS_COMPONENT_ID>();

				// setup for picking in the shaders
				for (uint32_t x = 0; x < sb.m_Mesh->getMeshBufferCount(); x++)
				{
					sb.m_Mesh->getMeshBuffer(x)->getMaterial().ObjectId = id.Id;
					sb.m_Mesh->getMeshBuffer(x)->getMaterial().Hilite = false;
				}

				KLS_Transform transform = temp.getAbsoluteTransform();
				if (sb.m_Mesh->hasTransparency())
				{
					getECS()->getLevel()->m_Transparent.push_back(KLS_RenderMesh(sb.m_Mesh, transform,cd.m_CameraDistance));
				}
				else
				{
					getECS()->getLevel()->m_Solid.push_back(KLS_RenderMesh(sb.m_Mesh, transform,cd.m_CameraDistance));
				}
			}
		}
	}

	// skybox system
	KLS_ECS_System_Skybox::KLS_ECS_System_Skybox(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Skybox::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_SKYBOX>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_SKYBOX& sb = getECS()->getRegistry().get<KLS_COMPONENT_SKYBOX>(entity);
			if (sb.m_Mesh)
			{
				KLS_Entity e(getECS(), entity);
				KLS_COMPONENT_ID& id = e.getComponent<KLS_COMPONENT_ID>();
				for (uint32_t x = 0; x < sb.m_Mesh->getMeshBufferCount(); x++)
					sb.m_Mesh->getMeshBuffer(x)->getMaterial().ObjectId = id.Id;
				KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
				t.m_Transform.setPosition(getECS()->getLevel()->getDriver()->getCameraPosition());
				getECS()->getLevel()->m_Skyboxes.push_back(KLS_RenderMesh(sb.m_Mesh, t.m_Transform,0));
			}
		}
	}

	// physx system
	KLS_ECS_System_PhysX::KLS_ECS_System_PhysX(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_PhysX::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_PHYSXOBJECT>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_PHYSXOBJECT& po = getECS()->getRegistry().get<KLS_COMPONENT_PHYSXOBJECT>(entity);
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
				glm::vec3 pos;
				glm::quat rot;
					po.m_PhysxObject->getPositionAndRotation(pos, rot);
				t.m_Transform.setPosition(pos);
				t.m_Transform.setRotation(rot);
		}
	}

	// sit and rotate system
	KLS_ECS_System_Rotate::KLS_ECS_System_Rotate(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Rotate::tick(const double& elapsedtime)
	{
		return;
		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_ROTATE>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			KLS_COMPONENT_ROTATE& r = getECS()->getRegistry().get<KLS_COMPONENT_ROTATE>(entity);

			r.m_TotalTime += elapsedtime;

			// Calculate the rotation angles around each axis based on elapsed time
			//float xRotation = static_cast<float>(r.m_TotalTime) * r.m_RotateSpeed.x;
			float yRotation = static_cast<float>(r.m_TotalTime) * r.m_RotateSpeed.y;
			//float zRotation = static_cast<float>(r.m_TotalTime) * r.m_RotateSpeed.z;

			// Create quaternions for each axis
			//glm::quat xQuat = glm::angleAxis(xRotation, glm::vec3(1.0f, 0.0f, 0.0f)); // Rotation around the x-axis
			glm::quat yQuat = glm::angleAxis(yRotation, glm::vec3(0.0f, 1.0f, 0.0f)); // Rotation around the y-axis
			//glm::quat zQuat = glm::angleAxis(zRotation, glm::vec3(0.0f, 0.0f, 1.0f)); // Rotation around the z-axis

			// Combine the quaternions to represent the total rotation
			glm::quat totalRotation = yQuat;// *xQuat* zQuat; // Change the order if needed

			// Apply the rotation to your object's transformation
			t.m_Transform.setRotation(totalRotation);
		}
	}

	// flycircle
	KLS_ECS_System_FlyCircle::KLS_ECS_System_FlyCircle(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_FlyCircle::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_FLYCIRCLE>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			KLS_COMPONENT_FLYCIRCLE& fc = getECS()->getRegistry().get<KLS_COMPONENT_FLYCIRCLE>(entity);

			if (fc.m_First)
			{
				fc.m_First = false;
				glm::normalize(fc.m_Direction);
				if (fc.m_Direction.y != 0)
				{
					fc.m_VecV = glm::cross(glm::vec3(1, 0, 0), fc.m_Direction);
					glm::normalize(fc.m_VecV);
				}
				else
				{
					fc.m_VecV = glm::cross(glm::vec3(0, 1, 0), fc.m_Direction);
					glm::normalize(fc.m_VecV);
				}
				fc.m_VecU = glm::cross(fc.m_VecV, fc.m_Direction);
				glm::normalize(fc.m_VecU);
			}

			fc.m_StartTime += fc.m_Speed * (float)elapsedtime;
#if(1)
			t.m_Transform.setPosition(fc.m_Center + fc.m_Radius * ((fc.m_VecU * cosf(fc.m_StartTime)) + (fc.m_VecV * sinf(fc.m_StartTime))));
#else
			float r2 = fc.m_RadiusEllipsoid == 0.f ? fc.m_Radius : fc.m_RadiusEllipsoid;
			t.m_Transform.setPosition(fc.m_Center + (fc.m_Radius * cosf(fc.m_StartTime) * fc.m_VecU) + (r2 * sinf(fc.m_StartTime) * fc.m_VecV));
#endif
		}
	}

	// keyboard controller
	KLS_ECS_System_Controller::KLS_ECS_System_Controller(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Controller::tick(const double& elapsedtime)
	{
		KLS_Driver* driver = getECS()->getLevel()->getDriver();

		auto view = getECS()->getView<KLS_COMPONENT_TRANSFORM, KLS_COMPONENT_CONTROLLER>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_TRANSFORM& t = getECS()->getRegistry().get<KLS_COMPONENT_TRANSFORM>(entity);
			KLS_COMPONENT_CONTROLLER& c = getECS()->getRegistry().get<KLS_COMPONENT_CONTROLLER>(entity);

			// calculate the movement velocity
			float velocity = c.MovementSpeed * (float)elapsedtime;

			// check for movement
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_FORWARD))) c.MoveForward = true; else c.MoveForward = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_BACKWARD))) c.MoveBackward = true; else c.MoveBackward = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_TURNLEFT))) c.MoveTurnLeft = true; else c.MoveTurnLeft = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_TURNRIGHT))) c.MoveTurnRight = true; else c.MoveTurnRight = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_STRAFELEFT))) c.MoveStrafeLeft = true; else c.MoveStrafeLeft = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_STRAFERIGHT))) c.MoveStrafeRight = true; else c.MoveStrafeRight = false;
			if (driver->IsKeyPressed(c.KeyMap.getKey(KLS_ACTION::KLS_MOVE_RUN))) velocity *= c.MovementMultipler;

			glm::vec3 position = t.m_Transform.getPosition();
			glm::quat rotation = t.m_Transform.getRotation();

			// handle the keyboard inputs
			if (c.MoveForward)		position += t.m_Transform.getIn() * velocity;
			if (c.MoveBackward)		position -= t.m_Transform.getIn() * velocity;
			if (c.MoveStrafeLeft)	position -= t.m_Transform.getLeft() * velocity;
			if (c.MoveStrafeRight)	position += t.m_Transform.getLeft() * velocity;

			float angle = c.MovementTurnSpeed * c.MovementTurnSpeed;
			if (c.MoveTurnLeft)
			{
				glm::quat rotationChange = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = rotationChange * rotation;
			}

			if (c.MoveTurnRight)
			{
				glm::quat rotationChange = glm::angleAxis(-angle, glm::vec3(0.0f, 1.0f, 0.0f));
				rotation = rotationChange * rotation;
			}

			t.m_Transform.setRotation(rotation);
			t.m_Transform.setPosition(position);
		}
	}

	// death timer
	KLS_ECS_System_DeathTimer::KLS_ECS_System_DeathTimer(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_DeathTimer::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_DEATHTIMER>();
		for (entt::entity entity : view)
		{
			KLS_Entity e(getECS(), entity);
			auto& dt = e.getComponent<KLS_COMPONENT_DEATHTIMER>();
			dt.Timer -= (float)elapsedtime;
			if (dt.Timer <= 0) getECS()->removeEntity(e);
		}
	}

	// editor hilite
	KLS_ECS_System_Hilite::KLS_ECS_System_Hilite(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Hilite::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_HILITE>();
		for (entt::entity entity : view)
		{
			KLS_Entity e(getECS(), entity);
			if (e.hasComponent<KLS_COMPONENT_MESH>())
			{
				auto& dt = e.getComponent<KLS_COMPONENT_HILITE>();
				auto& mesh = e.getComponent<KLS_COMPONENT_MESH>();
				if (mesh.m_Mesh)
				{
					if (mesh.m_Mesh->getMeshBufferCount() > (uint32_t)dt.MeshBuffer)
						mesh.m_Mesh->getMeshBuffer(dt.MeshBuffer)->getMaterial().Hilite = true;
					else
						dt.MeshBuffer = 0;
				}
			}
		}
	}

	// editor debug
	KLS_ECS_System_Debug::KLS_ECS_System_Debug(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Debug::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_DEBUG>();
		for (entt::entity entity : view)
		{
			KLS_Entity e(getECS(), entity);
			auto& d = e.getComponent<KLS_COMPONENT_DEBUG>();
			if (d.Active)
			{
				KLS_COMPONENT_CAMERA_DISTANCE& cd = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA_DISTANCE>(entity);
				if (cd.m_CameraDistance < getECS()->getLevel()->getCullDistance())
				{
					bool dirty = e.getTransform().m_Dirty;
					KLS_Transform transform = e.getAbsoluteTransform();
					KLS_AABBBox box = { glm::vec3(0), transform.getScale() };


					if ((dirty) && (e.hasComponent<KLS_COMPONENT_MESH>()))
					{
						auto& mc = e.getComponent<KLS_COMPONENT_MESH>();
						if (mc.m_Mesh)
						{
							box = mc.m_Mesh->updateBoundingBox();
						}
					}

					if (!d.m_Mesh)
					{
						if (e.hasComponent<KLS_COMPONENT_MESH>())
						{
							auto& mc = e.getComponent<KLS_COMPONENT_MESH>();
							if (mc.m_Mesh)
							{
								box = mc.m_Mesh->updateBoundingBox();
							}
						}

						d.m_Mesh = getECS()->getLevel()->getDriver()->getResourceManager()->createMesh(KLSVT3D, "bbox",
							createVertices_Cube(box.m_Min, box.m_Max),
							createIndices_Cube());
						d.m_Mesh->getMaterial(0).MaterialType = KLSMT_COLOR;
						d.m_Mesh->getMaterial(0).Wireframe = true;
					}

					getECS()->getLevel()->m_Solid.push_back(KLS_RenderMesh(d.m_Mesh, transform, cd.m_CameraDistance));
				}
			}
		}
	}

} // end namespace