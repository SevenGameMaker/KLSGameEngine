// include the needed header files
#include "KLS_Camera.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"

// unclutter the gloal namespace
namespace KLS
{
	// class constructor
	KLS_Camera::KLS_Camera(KLS_Driver* driver)
	{
		m_Driver = driver;
		m_Up = { 0.0f, 1.0f, 0.0f };
		m_Front = { 0.0f, 0.0f, 0.0f };
		m_Right = { 0.0f, 0.0f, 0.0f };
		m_Target = { 0.0f, 0.0f, 0.0f };
		updateCameraVectors();
	}

	// class destructor
	KLS_Camera::~KLS_Camera()
	{
		m_Driver = nullptr;
	}

	void KLS_Camera::setActive(bool v)
	{
		m_Active = v;
	}

	glm::vec3 KLS_Camera::getIn()
	{
		glm::vec3 in = m_Target - m_Transform.getPosition();
		in = glm::normalize(in);
		return in;
	}

	void KLS_Camera::setFOV(float v)
	{
		m_FOV = v;
		if (m_FOV > m_MaxFOV) m_FOV = m_MaxFOV;
		if (m_FOV < m_MinFOV) m_FOV = m_MinFOV;
		KLS_ERROR("fov = {}", m_FOV);
	}

	void KLS_Camera::updateCameraVectors()
	{
		glm::vec3 position = m_Transform.getPosition();
		if (m_TargetSet)
		{
			glm::vec3 direction = glm::normalize(m_Target - position);
			m_Yaw = glm::degrees(atan2(direction.z, direction.x));
			m_Pitch = glm::degrees(asin(direction.y));
			m_Front = glm::normalize(direction);
			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
			m_TargetSet = false;
		}
		else
		{
			m_Front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front.y = sin(glm::radians(m_Pitch));
			m_Front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
			m_Front = glm::normalize(m_Front);

			m_Right = glm::normalize(glm::cross(m_Front, m_WorldUp));
			m_Up = glm::normalize(glm::cross(m_Right, m_Front));
			m_Target = position + m_Front;
		}
	}

	void KLS_Camera::onAnimate(const double& elapsedtime)
	{
		updateCameraVectors();
	}

	void KLS_Camera::setPositionAndPitch(glm::vec3 pos, float pitch)
	{
		m_Transform.setPosition(pos);
		setPitch(pitch);
		updateCameraVectors();
	}

	void KLS_Camera::setPosition(glm::vec3 pos)
	{
		m_Transform.setPosition(pos);
		updateCameraVectors();
	}

	void KLS_Camera::setTarget(const glm::vec3& newTarget)
	{
		m_TargetSet = true;
		m_Target = newTarget;
		updateCameraVectors();
	}

	glm::mat4 KLS_Camera::getViewMatrix()
	{
		m_ViewMatrix = glm::lookAt(m_Transform.getPosition(), m_Transform.getPosition() + m_Front, m_Up);
		return m_ViewMatrix;
	}

	glm::mat4 KLS_Camera::getProjMatrix(float aspectratio)
	{
		m_ProjMatrix = glm::perspective(m_FOV, aspectratio, m_NearPlane, m_FarPlane);
		return m_ProjMatrix;
	}

	glm::mat4 KLS_Camera::getOrthoMatrix(int width, int height)
	{
		m_OrthoMatrix = glm::ortho(0.0f,
			static_cast<float>(width),
			static_cast<float>(height),
			0.0f, -1.0f, 1.0f);
		return m_OrthoMatrix;
	}

} // end namespace