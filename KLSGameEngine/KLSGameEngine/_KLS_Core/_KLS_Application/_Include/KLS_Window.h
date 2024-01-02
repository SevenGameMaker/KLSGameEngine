// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"
#include "KLS_EventHandler.h"
#include "KLS_ApplicationCreationInfo.h"

#include <glad/gl.h> // we have to add this event if we arent using it so that glad is added before glfw
#include <GLFW/glfw3.h>

// unclutter the global namespace
namespace KLS
{
	/*
	*
	*/
	class KLS_API KLS_Window
	{
	private:
		KLS_VARIABLE_GET(GLFWwindow*, Window, nullptr);
		KLS_VARIABLE_GET(KLS_EventHandler*, EventHandler, nullptr);

		KLS_VARIABLE_SETGET(int, Width, 0);											// current window width
		KLS_VARIABLE_SETGET(int, Height, 0);										// current window height
		KLS_VARIABLE_SETGET(float, Ratio, 0.0f);									// current window aspect ration
		KLS_VARIABLE_GET(KLS_VSYNC_FLAGS, VSync, KLS_VSYNC_FLAGS::KLSVS_NONE);		// vsync value

	public:
		// class constructor / destructor (use create / cleanup instead)
		KLS_Window();
		virtual ~KLS_Window();

		// dual creation allows for better error handling
		virtual bool create(KLS_EventHandler* eventhandler, KLS_ApplicationCreationInfo& params);

		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// let the glfw window poll for events
		void pollEvents();

		// send an event tot he event handler
		void sendEvent(const KLS_Event& e);

		// swap the internal buffers
		virtual void swapBuffers();

		// window interaction
		virtual void setCaption(std::string caption);

		// cursor interaction
		virtual void setCursorPosition(glm::vec2 p);

		// cursor interaction
		virtual void setCursorVisible(KLS_CURSOR_TYPE t);

		// center position of window (in our case for the fpscamera use)
		virtual glm::vec2 getCenter();

		// window close flag
		virtual bool shouldClose();

		// set the vsync of the window
		virtual void setVSync(KLS_VSYNC_FLAGS flag);

		// force this class to update the window position and size variables
		virtual void forceResizeEvent();

		// make the window the current context
		virtual void makeCurrentContext();

	private:
		// callback functions 
		// glfw callback functions
		static void callback_error(int error_code, const char* description);
		static void callback_windowpos(GLFWwindow* window, int xpos, int ypos);
		static void callback_windowsize(GLFWwindow* window, int width, int height);
		static void callback_close(GLFWwindow* window);
		static void callback_windowfocus(GLFWwindow* window, int focused);
		static void callback_windowiconify(GLFWwindow* window, int iconified);
		static void callback_windowmaximize(GLFWwindow* window, int maximized);
		static void callback_framebufferresize(GLFWwindow* window, int width, int height);
		static void callback_mouse(GLFWwindow* window, int button, int action, int mods);
		static void callback_mousewheel(GLFWwindow* window, double xoffset, double yoffset);
		static void callback_mousemove(GLFWwindow* window, double xpos, double ypos);
		static void callback_mouseenter(GLFWwindow* window, int entered);
		static void callback_keyboard(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void callback_filenamedrop(GLFWwindow* window, int path_count, const char* paths[]);
	};


} // end namespace