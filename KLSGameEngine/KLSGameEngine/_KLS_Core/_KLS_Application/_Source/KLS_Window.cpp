// include the needed header files
#include "KLS_Window.h"
#include "KLS_Logger.h"
#include "KLS_String.h"

// unclutter the global namespace
namespace KLS
{
	// class constructor / destructor (use create / cleanup instead)
	KLS_Window::KLS_Window()
	{
		// log this activity
		KLS_TRACE("KLS_Window::KLS_Window()");
	}

	KLS_Window::~KLS_Window()
	{
		// log this activity
		KLS_TRACE("KLS_Window::~KLS_Window()");
	}

	// dual creation allows for better error handling
	bool KLS_Window::create(KLS_EventHandler* eventhandler, KLS_ApplicationCreationInfo& info)
	{
		// log this activity
		KLS_TRACE("KLS_Window::create()");

		// remember this
		m_EventHandler = eventhandler;

		// initialize the glfw system
		KLS_CHECK(glfwInit(), "glfwInit() failed", KLS_FAILURE);

		switch (info.m_DriverType)
		{
		case KLSDT_OPENGL:
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, KLS_OPENGL_MAJOR_VERSION);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, KLS_OPENGL_MINOR_VERSION);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
			glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
			glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
			glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);

			glfwWindowHint(GLFW_RESIZABLE, info.m_Resizeable);
			glfwWindowHint(GLFW_DECORATED, info.m_DecoratedWindow);
			glfwWindowHint(GLFW_SAMPLES, info.m_MultiSamples);
			glfwWindowHint(GLFW_DEPTH_BITS, info.m_DepthBits);
			glfwWindowHint(GLFW_STENCIL_BITS, info.m_StencilBits);
		}	break;

		default:
			KLS_ERROR("driver type not supported yet...");
			return KLS_FAILURE;
		}

		if (info.m_Fullscreen)
		{
			// Get the primary monitor's video mode
			GLFWmonitor* monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(monitor);
			info.m_WindowPosition.x = 0;
			info.m_WindowPosition.z = mode->width;
			info.m_WindowPosition.y = 0;
			info.m_WindowPosition.w = mode->height;
		}

		// Create a GLFW window
		m_Window = glfwCreateWindow(
			info.m_WindowPosition.z - info.m_WindowPosition.x,
			info.m_WindowPosition.w - info.m_WindowPosition.y,
			info.m_Title.c_str(), nullptr, nullptr);

		// make sure the window creation did not fail
		KLS_CHECK((m_Window != nullptr), "glfwCreateWindow() failed", KLS_FAILURE);

		glfwMakeContextCurrent(m_Window);

		// Set the user pointer to this instance for the callback to access it
		glfwSetWindowUserPointer(m_Window, this);

		// stup the callbacks
		glfwSetErrorCallback(callback_error);
		glfwSetWindowPosCallback(m_Window, callback_windowpos);
		glfwSetWindowSizeCallback(m_Window, callback_windowsize);
		glfwSetWindowCloseCallback(m_Window, callback_close);
		glfwSetWindowFocusCallback(m_Window, callback_windowfocus);
		glfwSetWindowIconifyCallback(m_Window, callback_windowiconify);
		glfwSetWindowMaximizeCallback(m_Window, callback_windowmaximize);
		glfwSetFramebufferSizeCallback(m_Window, callback_framebufferresize);
		glfwSetMouseButtonCallback(m_Window, callback_mouse);
		glfwSetScrollCallback(m_Window, callback_mousewheel);
		glfwSetCursorPosCallback(m_Window, callback_mousemove);
		glfwSetCursorEnterCallback(m_Window, callback_mouseenter);
		glfwSetKeyCallback(m_Window, callback_keyboard);
		glfwSetDropCallback(m_Window, callback_filenamedrop);

		if (!info.m_Fullscreen)
		{
			glfwSetWindowPos(m_Window, info.m_WindowPosition.x, info.m_WindowPosition.y);
			glfwSetWindowSize(m_Window, info.m_WindowPosition.z, info.m_WindowPosition.w);
		}

		setWidth(info.m_WindowPosition.z - info.m_WindowPosition.x);
		setHeight(info.m_WindowPosition.w - info.m_WindowPosition.y);
		setRatio((float)m_Width / float(m_Height));

		setVSync(info.m_VSync);

		// everything went fine
		return KLS_SUCCESS;
	}

	// cleanup whatever memory mess we made
	bool KLS_Window::cleanup()
	{
		// log this activity
		KLS_TRACE("KLS_Window::cleanup()");

		// forget this
		m_EventHandler = nullptr;

		// safely etroy the window
		if (m_Window) glfwDestroyWindow(m_Window); m_Window = nullptr;

		// shut down the glfw system
		glfwTerminate();

		// always return false from a cleanup function to allow for cascading errors
		return KLS_FAILURE;
	}

	// let the glfw window poll for events
	void KLS_Window::pollEvents()
	{
		glfwPollEvents();
	}

	// window close flag
	bool KLS_Window::shouldClose()
	{
		return glfwWindowShouldClose(m_Window);
	}

	// set the vsyn of the window
	void KLS_Window::setVSync(KLS_VSYNC_FLAGS flag)
	{
		if (m_VSync == flag) return;

		m_VSync = flag;
		glfwMakeContextCurrent(m_Window);

		switch (m_VSync)
		{
		case KLS_VSYNC_FLAGS::KLSVS_LIMITED: glfwSwapInterval(1); break;
		case KLS_VSYNC_FLAGS::KLSVS_PROTECTED: glfwSwapInterval(1); break;
		case KLS_VSYNC_FLAGS::KLSVS_UNLIMITED: glfwSwapInterval(0); break;
		}
	}

	void KLS_Window::forceResizeEvent()
	{
		glfwGetWindowSize(m_Window, &m_Width, &m_Height);
		setRatio((float)m_Width / float(m_Height));

		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_FRAMEBUFFERRESIZED;
		e.System.data0 = m_Width;
		e.System.data1 = m_Height;
		sendEvent(e);
	}

	void KLS_Window::makeCurrentContext()
	{
		glfwMakeContextCurrent(m_Window);
	}

	// swap the internal buffers
	void KLS_Window::swapBuffers()
	{
		glfwSwapBuffers(m_Window);
	}

	void KLS_Window::setCaption(std::string caption)
	{
		glfwSetWindowTitle(m_Window, caption.c_str());
	}

	glm::vec2 KLS_Window::getCenter()
	{
		return glm::vec2((m_Width) / 2, (m_Height) / 2);
	}

	void KLS_Window::setCursorPosition(glm::vec2 p)
	{
		glfwSetCursorPos(m_Window, p.x, p.y);
	}

	// cursor interaction
	void KLS_Window::setCursorVisible(KLS_CURSOR_TYPE t)
	{
		int glfwCursorType;

		// Map your custom enum values to GLFW cursor constants
		switch (t) 
		{
		case KLSCT_NORMAL:
			glfwCursorType = GLFW_CURSOR_NORMAL;
			break;
		case KLSCT_HIDDEN:
			glfwCursorType = GLFW_CURSOR_HIDDEN;
			break;
		case KLSCT_DISABLED:
			glfwCursorType = GLFW_CURSOR_DISABLED;
			break;
		case KLSCT_ARROW:
			glfwCursorType = GLFW_ARROW_CURSOR;
			break;
			// Add cases for other cursor types you've defined
		default:
			// Handle unrecognized cursor type
			glfwCursorType = GLFW_CURSOR_NORMAL; // Default to normal cursor
			break;
		}

		glfwSetInputMode(m_Window, GLFW_CURSOR, glfwCursorType);
	}

	// send an event to the event handler
	void KLS_Window::sendEvent(const KLS_Event& e)
	{
		if (m_EventHandler) m_EventHandler->onEvent(e);
	}

	void KLS_Window::callback_error(int error_code, const char* description)
	{
		KLS_WARN("GLFW error : {} {}", error_code, description);
	}

	void KLS_Window::callback_windowpos(GLFWwindow* window, int xpos, int ypos)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_WINDOWMOVED;
		e.System.data0 = xpos;
		e.System.data1 = ypos;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_windowsize(GLFWwindow* window, int width, int height)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));

		wrapper->setWidth(width);
		wrapper->setHeight(height);
		wrapper->setRatio((float)width / float(height));

		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_WINDOWSIZED;
		e.System.data0 = width;
		e.System.data1 = height;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_close(GLFWwindow* window)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_CLOSE;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_windowfocus(GLFWwindow* window, int focused)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_WINDOWFOCUS;
		e.System.data0 = focused;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_windowiconify(GLFWwindow* window, int iconified)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_WINDOWICONIFIED;
		e.System.data0 = iconified;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_windowmaximize(GLFWwindow* window, int maximized)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_WINDOWMAXIMIZE;
		e.System.data0 = maximized;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_framebufferresize(GLFWwindow* window, int width, int height)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));

		wrapper->setWidth(width);
		wrapper->setHeight(height);
		wrapper->setRatio((float)width / float(height));

		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_FRAMEBUFFERRESIZED;
		e.System.data0 = width;
		e.System.data1 = height;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_mouse(GLFWwindow* window, int button, int action, int mods)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_MOUSE;

		if (action == GLFW_PRESS)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT) e.Mouse.EventType = KLS_EVT_LMOUSE_PRESSED;
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) e.Mouse.EventType = KLS_EVT_MMOUSE_PRESSED;
			if (button == GLFW_MOUSE_BUTTON_RIGHT) e.Mouse.EventType = KLS_EVT_RMOUSE_PRESSED;
		}
		else if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT) e.Mouse.EventType = KLS_EVT_LMOUSE_RELEASED;
			if (button == GLFW_MOUSE_BUTTON_MIDDLE) e.Mouse.EventType = KLS_EVT_MMOUSE_RELEASED;
			if (button == GLFW_MOUSE_BUTTON_RIGHT) e.Mouse.EventType = KLS_EVT_RMOUSE_RELEASED;
		}

		e.Mouse.Control = mods & GLFW_MOD_CONTROL;
		e.Mouse.Shift = mods & GLFW_MOD_SHIFT;
		e.Mouse.X = KLS_EventHandler::MouseX();
		e.Mouse.Y = KLS_EventHandler::MouseY();

		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_mousemove(GLFWwindow* window, double xpos, double ypos)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_MOUSE;
		e.Mouse.EventType = KLS_EVT_MOUSE_MOVED;
		e.Mouse.Control = KLS_EventHandler::IsKeyPressed(KEY_LEFT_CONTROL);
		e.Mouse.Shift = KLS_EventHandler::IsKeyPressed(KEY_LEFT_SHIFT);
		e.Mouse.X = (int)xpos;
		e.Mouse.Y = (int)ypos;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_mousewheel(GLFWwindow* window, double xoffset, double yoffset)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_MOUSE;
		e.Mouse.EventType = KLS_EVT_MOUSE_WHEEL;
		e.Mouse.Control = KLS_EventHandler::IsKeyPressed(KEY_LEFT_CONTROL);
		e.Mouse.Shift = KLS_EventHandler::IsKeyPressed(KEY_LEFT_SHIFT);
		e.Mouse.X = (int)KLS_EventHandler::MouseX();
		e.Mouse.Y = (int)KLS_EventHandler::MouseY();
		e.Mouse.Wheel = (int)yoffset;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_mouseenter(GLFWwindow* window, int entered)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		if (entered) e.System.EventType = KLS_SYSTEM_MOUSE_ENTER;
		else e.System.EventType = KLS_SYSTEM_MOUSE_EXIT;
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_KEYBOARD;
		e.Keyboard.Key = key;
		e.Keyboard.Scancode = scancode;
		e.Keyboard.KeyDown = ((action == GLFW_REPEAT) || (action == GLFW_PRESS));
		wrapper->sendEvent(e);
	}

	void KLS_Window::callback_filenamedrop(GLFWwindow* window, int path_count, const char* paths[])
	{
		KLS_Window* wrapper = static_cast<KLS_Window*>(glfwGetWindowUserPointer(window));
		KLS_Event e;
		e.EventType = KLS_EVT_SYSTEM;
		e.System.EventType = KLS_SYSTEM_FILENAMEDROP;
		wrapper->sendEvent(e);
	}

} // end namespace