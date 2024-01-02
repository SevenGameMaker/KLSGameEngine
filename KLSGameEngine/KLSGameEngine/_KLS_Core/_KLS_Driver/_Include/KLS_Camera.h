// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_Transform.h"
#include "KLS_KeyMap.h"

// unclutter the global namespace
namespace KLS
{
	typedef class KLS_Driver KLS_Driver;

	class KLS_API KLS_Camera
	{
	private:
		KLS_VARIABLE_GET(KLS_Driver*, Driver, nullptr);
		KLS_VARIABLE_EDITABLE(KLS_Transform, Transform, KLS_Transform());

		KLS_VARIABLE_SETGET(glm::vec3, Up, glm::vec3(0, 1, 0));
		KLS_VARIABLE_SETGET(glm::vec3, WorldUp, glm::vec3(0, 1, 0));
		KLS_VARIABLE_SETGET(glm::vec3, Front, glm::vec3(0, 0, 0));
		KLS_VARIABLE_SETGET(glm::vec3, Right, glm::vec3(0, 0, 0));
		KLS_VARIABLE_SETGET(float, Yaw, 0);
		KLS_VARIABLE_SETGET(float, Pitch, 0);
		KLS_VARIABLE_EDITABLE_SETGET(float, NearPlane, 0.1f);
		KLS_VARIABLE_EDITABLE_SETGET(float, FarPlane, 100.0f);

		KLS_VARIABLE_GET(float, FOV, 45);
		KLS_VARIABLE_GET(float, MinFOV, 44.5);
		KLS_VARIABLE_GET(float, MaxFOV, 45.5);

		KLS_VARIABLE_GET(glm::vec3, Target, glm::vec3(0, 0, 0));
		KLS_VARIABLE_GET(bool, TargetSet, false);

		KLS_VARIABLE_GET(bool, Active, true);

		glm::mat4 m_ViewMatrix = glm::mat4(0);
		glm::mat4 m_ProjMatrix = glm::mat4(0);
		glm::mat4 m_OrthoMatrix = glm::mat4(0);

	public:
		// class constuctor
		KLS_Camera(KLS_Driver* driver);

		// class destructor
		virtual ~KLS_Camera();

		// frame emthod
		virtual void onAnimate(const double& elapsedtime);
		virtual void updateCameraVectors();

		// get the matricies
		glm::mat4 getViewMatrix();
		glm::mat4 getProjMatrix(float aspectratio);
		glm::mat4 getOrthoMatrix(int width, int height);
		KLS_Transform& getTransform() { return m_Transform; }

		// set the variables
		void setTarget(const glm::vec3& newTarget);
		virtual void setPositionAndPitch(glm::vec3 pos, float pitch);
		virtual void setPosition(glm::vec3 pos);
		virtual void setActive(bool v);
		virtual void setFOV(float v);

		glm::vec3 getIn();
	};

} // end namespace