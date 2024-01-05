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

		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_CONTROLLER>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
			KLS_COMPONENT_CAMERA& c = getECS()->getRegistry().get<KLS_COMPONENT_CAMERA>(entity);
			c.Camera->onAnimate(elapsedtime);
			t.Transform = c.Camera->getTransform();
		}
	};


	// update the disance to the camera for all entities
	KLS_ECS_System_CameraDistance::KLS_ECS_System_CameraDistance(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_CameraDistance::tick(const double& elapsedtime)
	{
		glm::vec3 camPosition = getECS()->getLevel()->getDriver()->getCameraPosition();
		auto view = getECS()->getView<KLS_COMPONENT_INFO>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);

			KLS_Entity temp(getECS(), entity);
			KLS_Transform transform = temp.getAbsoluteTransform();
			glm::vec3 pos = transform.getPosition();

			t.CameraDistance = glm::distance(camPosition, pos);
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
			KLS_COMPONENT_INFO& cd = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
			lc.LightData.position = glm::vec4(t.getPosition(), 1.0f);
			lc.LightData.cameraDistance = cd.CameraDistance;
			getECS()->getLevel()->m_Lights.push_back(KLS_RenderLight(lc.LightData));
		}
	}

	// mesh system
	KLS_ECS_System_Mesh::KLS_ECS_System_Mesh(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Mesh::tick(const double& elapsedtime)
	{
		auto view = getECS()->getRegistry().view<KLS_COMPONENT_INFO, KLS_COMPONENT_MESH>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
			KLS_COMPONENT_MESH& sb = getECS()->getRegistry().get<KLS_COMPONENT_MESH>(entity);
			
			if ((sb.Mesh) && (t.CameraDistance < getECS()->getLevel()->getCullDistance()))
			{
				KLS_Entity temp(getECS(), entity);

				// setup for picking in the shaders
				for (uint32_t x = 0; x < sb.Mesh->getMeshBufferCount(); x++)
				{
					sb.Mesh->getMeshBuffer(x)->getMaterial().ObjectId = t.Id;
					sb.Mesh->getMeshBuffer(x)->getMaterial().Hilite = false;
				}

				KLS_Transform transform = temp.getAbsoluteTransform();
				if (sb.Mesh->hasTransparency())
				{
					getECS()->getLevel()->m_Transparent.push_back(KLS_RenderMesh(sb.Mesh, transform,t.CameraDistance));
				}
				else
				{
					getECS()->getLevel()->m_Solid.push_back(KLS_RenderMesh(sb.Mesh, transform,t.CameraDistance));
				}
			}
		}
	}

	// skybox system
	KLS_ECS_System_Skybox::KLS_ECS_System_Skybox(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Skybox::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_SKYBOX>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& i = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
			KLS_COMPONENT_SKYBOX& sb = getECS()->getRegistry().get<KLS_COMPONENT_SKYBOX>(entity);
			if (sb.Mesh)
			{
				KLS_Entity e(getECS(), entity);
				for (uint32_t x = 0; x < sb.Mesh->getMeshBufferCount(); x++)
					sb.Mesh->getMeshBuffer(x)->getMaterial().ObjectId = i.Id;
				i.Transform.setPosition(getECS()->getLevel()->getDriver()->getCameraPosition());
				getECS()->getLevel()->m_Skyboxes.push_back(KLS_RenderMesh(sb.Mesh, i.Transform,0));
			}
		}
	}

	// physx system
	KLS_ECS_System_PhysX::KLS_ECS_System_PhysX(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_PhysX::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_PHYSXOBJECT>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_PHYSXOBJECT& po = getECS()->getRegistry().get<KLS_COMPONENT_PHYSXOBJECT>(entity);
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
				glm::vec3 pos;
				glm::quat rot;
					po.PhysXObject->getPositionAndRotation(pos, rot);
				t.Transform.setPosition(pos);
				t.Transform.setRotation(rot);
		}
	}

	// sit and rotate system
	KLS_ECS_System_Rotate::KLS_ECS_System_Rotate(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_Rotate::tick(const double& elapsedtime)
	{
		return;
		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_ROTATE>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
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
			t.Transform.setRotation(totalRotation);
		}
	}

	// flycircle
	KLS_ECS_System_FlyCircle::KLS_ECS_System_FlyCircle(KLS_ECS* ecs) : KLS_ECS_System(ecs) {}
	void KLS_ECS_System_FlyCircle::tick(const double& elapsedtime)
	{
		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_FLYCIRCLE>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
			KLS_COMPONENT_FLYCIRCLE& fc = getECS()->getRegistry().get<KLS_COMPONENT_FLYCIRCLE>(entity);

			if (fc.First)
			{
				fc.First = false;
				glm::normalize(fc.Direction);
				if (fc.Direction.y != 0)
				{
					fc.VecV = glm::cross(glm::vec3(1, 0, 0), fc.Direction);
					glm::normalize(fc.VecV);
				}
				else
				{
					fc.VecV = glm::cross(glm::vec3(0, 1, 0), fc.Direction);
					glm::normalize(fc.VecV);
				}
				fc.VecU = glm::cross(fc.VecV, fc.Direction);
				glm::normalize(fc.VecU);
			}

			fc.StartTime += fc.Speed * (float)elapsedtime;
#if(1)
			t.Transform.setPosition(fc.Center + fc.Radius * ((fc.VecU * cosf(fc.StartTime)) + (fc.VecV * sinf(fc.StartTime))));
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

		auto view = getECS()->getView<KLS_COMPONENT_INFO, KLS_COMPONENT_CONTROLLER>();
		for (entt::entity entity : view)
		{
			KLS_COMPONENT_INFO& t = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
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

			glm::vec3 position = t.Transform.getPosition();
			glm::quat rotation = t.Transform.getRotation();

			// handle the keyboard inputs
			if (c.MoveForward)		position += t.Transform.getIn() * velocity;
			if (c.MoveBackward)		position -= t.Transform.getIn() * velocity;
			if (c.MoveStrafeLeft)	position -= t.Transform.getLeft() * velocity;
			if (c.MoveStrafeRight)	position += t.Transform.getLeft() * velocity;

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

			t.Transform.setRotation(rotation);
			t.Transform.setPosition(position);
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
				if (mesh.Mesh)
				{
					if (mesh.Mesh->getMeshBufferCount() > (uint32_t)dt.MeshBuffer)
						mesh.Mesh->getMeshBuffer(dt.MeshBuffer)->getMaterial().Hilite = true;
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
		auto view = getECS()->getView<KLS_COMPONENT_DEBUGINFO>();
		for (entt::entity entity : view)
		{
			KLS_Entity e(getECS(), entity);
			auto& d = e.getComponent<KLS_COMPONENT_DEBUGINFO>();
			if (d.Active)
			{
				KLS_COMPONENT_INFO& cd = getECS()->getRegistry().get<KLS_COMPONENT_INFO>(entity);
				if (cd.CameraDistance < getECS()->getLevel()->getCullDistance())
				{
					bool dirty = e.getTransform().m_Dirty;
					KLS_Transform transform = e.getAbsoluteTransform();
					KLS_AABBBox box = { glm::vec3(0), transform.getScale() };


					if ((dirty) && (e.hasComponent<KLS_COMPONENT_MESH>()))
					{
						auto& mc = e.getComponent<KLS_COMPONENT_MESH>();
						if (mc.Mesh)
						{
							box = mc.Mesh->updateBoundingBox();
						}
					}

					if (!d.Mesh)
					{
						if (e.hasComponent<KLS_COMPONENT_MESH>())
						{
							auto& mc = e.getComponent<KLS_COMPONENT_MESH>();
							if (mc.Mesh)
							{
								box = mc.Mesh->updateBoundingBox();
							}
						}

						d.Mesh = getECS()->getLevel()->getDriver()->getResourceManager()->createMesh(KLSVT3D, "bbox",
							createVertices_Cube(box.m_Min, box.m_Max),
							createIndices_Cube());
						d.Mesh->getMaterial(0).MaterialType = KLSMT_COLOR;
						d.Mesh->getMaterial(0).Wireframe = true;
					}

					getECS()->getLevel()->m_Solid.push_back(KLS_RenderMesh(d.Mesh, transform, cd.CameraDistance));
				}
			}
		}
	}

} // end namespace