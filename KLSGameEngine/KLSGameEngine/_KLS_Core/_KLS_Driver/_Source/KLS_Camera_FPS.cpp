// include the needed header files
#include "KLS_Camera_FPS.h"
#include "KLS_Logger.h"
#include "_KLS_Driver.h"
#include "KLS_Window.h"

// unclutter the gloal namespace
namespace KLS
{
	// class constructor
	KLS_Camera_FPS::KLS_Camera_FPS(KLS_Driver* driver, KLS_KeyMap keymap) : KLS_Camera(driver)
	{
		m_KeyMap = keymap;
		m_FirstMouse = true;
		setActive(!m_Active);
		setActive(m_Active);
	}

	// class destructor
	KLS_Camera_FPS::~KLS_Camera_FPS()
	{
	}

	void KLS_Camera_FPS::setActive(bool v)
	{
		if (m_Active == v) return;
		KLS_Camera::setActive(v);

		if (m_Active) getDriver()->getWindow()->setCursorVisible(KLS_CURSOR_TYPE::KLSCT_HIDDEN);
		else getDriver()->getWindow()->setCursorVisible(KLS_CURSOR_TYPE::KLSCT_NORMAL);

		if (m_Active) m_FirstMouse = true;
	}

	void KLS_Camera_FPS::onAnimate(const double& elapsedtime)
	{
		KLS_Camera::onAnimate(elapsedtime);
		if (m_Active)
		{
			glm::vec2 wc = m_Driver->getWindow()->getCenter();
			m_MouseX = static_cast<float>(m_Driver->MouseX());
			m_MouseY = static_cast<float>(m_Driver->MouseY());

				// reposition the cursor to the cent of the screen
				m_Driver->getWindow()->setCursorPosition(wc);

				if (m_FirstMouse)
				{
					m_MouseX = (float)wc.x;
					m_MouseY = (float)wc.y;
					m_LastMouseX = (float)wc.x;
					m_LastMouseY = (float)wc.y;
					m_FirstMouse = false;
				}

				// handle the mouse cursor movement
				float xoffset = m_LastMouseX - m_MouseX;
				float yoffset = m_MouseY - m_LastMouseY;

				// remember these for next time
				m_LastMouseX = wc.x;
				m_LastMouseY = wc.y;

				// check for movement
				if (m_Driver->IsKeyPressed(m_KeyMap.getKey(KLS_ACTION::KLS_MOVE_FORWARD))) m_MoveForward = true; else m_MoveForward = false;
				if (m_Driver->IsKeyPressed(m_KeyMap.getKey(KLS_ACTION::KLS_MOVE_BACKWARD))) m_MoveBackward = true; else m_MoveBackward = false;
				if (m_Driver->IsKeyPressed(m_KeyMap.getKey(KLS_ACTION::KLS_MOVE_TURNLEFT))) m_MoveLeft = true; else m_MoveLeft = false;
				if (m_Driver->IsKeyPressed(m_KeyMap.getKey(KLS_ACTION::KLS_MOVE_TURNRIGHT))) m_MoveRight = true; else m_MoveRight = false;

				// calculate the movement velocity
				float velocity = m_MovementSpeed * (float)elapsedtime;

				// add sped multiplier
				if (m_Driver->IsKeyPressed(m_KeyMap.getKey(KLS_ACTION::KLS_MOVE_RUN))) velocity *= m_MovementMultipler;

				// temporary variables
				float yaw = getYaw();
				float pitch = getPitch();
				glm::vec3 position = getTransform().getPosition();

				xoffset *= m_MouseSensitivity * (float)elapsedtime;
				yoffset *= m_MouseSensitivity * (float)elapsedtime;
				yaw -= xoffset;
				pitch -= yoffset;

				// Clamp pitch to avoid flipping upside down
				pitch = glm::clamp(pitch, -89.0f, 89.0f);

				setYaw(yaw);
				setPitch(pitch);
				updateCameraVectors();

				glm::vec3 front = getFront();
				glm::vec3 right = getRight();

				// handle the keyboard inputs
				if (m_MoveForward)	position += front * velocity;
				if (m_MoveBackward)	position -= front * velocity;
				if (m_MoveLeft)		position -= right * velocity;
				if (m_MoveRight)	position += right * velocity;

				setFront(front);
				setRight(right);
				getTransform().setPosition(position);
		}
	}


} // end namespace