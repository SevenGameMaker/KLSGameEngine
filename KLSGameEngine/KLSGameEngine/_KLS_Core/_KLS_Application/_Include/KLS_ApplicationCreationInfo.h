// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	// simple strcuture to hold application creation information
	class KLS_API KLS_ApplicationCreationInfo
	{
	public:
		std::string			m_Title = "KLSGameEngine";						// window titlebar text
		KLS_DRIVER_TYPE		m_DriverType = KLSDT_OPENGL;					// driver type
		glm::uvec4			m_WindowPosition = glm::uvec4(0, 0, 800, 600);	// initial window position / size
		bool				m_Resizeable = true;							// is the window resizeable
		int					m_MultiSamples = 0;								// multisample 
		bool				m_DoubleBuffer = true;							// doublebuffer
		bool				m_DecoratedWindow = true;						// window style
		int					m_DepthBits = 24;								// depth bits
		int					m_StencilBits = 24;								// stencil bits
		KLS_VSYNC_FLAGS		m_VSync = KLSVS_UNLIMITED;						// vsynx
		bool				m_Fullscreen = false;							// fullscreen

	public:
		// default constructor
		KLS_ApplicationCreationInfo() { }

		// verbose constructor
		KLS_ApplicationCreationInfo(KLS_DRIVER_TYPE drivertype, glm::uvec4 windowposition, bool resizeable)
			: m_DriverType(drivertype), m_WindowPosition(windowposition), m_Resizeable(resizeable)
		{}

		// very verbose constructor
		KLS_ApplicationCreationInfo(const KLS_ApplicationCreationInfo& other)
			: m_Title(other.m_Title), m_DriverType(other.m_DriverType),
			m_WindowPosition(other.m_WindowPosition), m_Resizeable(other.m_Resizeable),
			m_MultiSamples(other.m_MultiSamples), m_DoubleBuffer(other.m_DoubleBuffer),
			m_DecoratedWindow(other.m_DecoratedWindow), m_DepthBits(other.m_DepthBits),
			m_StencilBits(other.m_StencilBits), m_VSync(other.m_VSync), m_Fullscreen(other.m_Fullscreen)
		{}

		// assignment operator
		KLS_ApplicationCreationInfo& operator=(const KLS_ApplicationCreationInfo& other)
		{
			if (this != &other) // Check for self-assignment
			{
				m_Title = other.m_Title;
				m_DriverType = other.m_DriverType;
				m_WindowPosition = other.m_WindowPosition;
				m_Resizeable = other.m_Resizeable;
				m_MultiSamples = other.m_MultiSamples;
				m_DoubleBuffer = other.m_DoubleBuffer;
				m_DecoratedWindow = other.m_DecoratedWindow;
				m_DepthBits = other.m_DepthBits;
				m_StencilBits = other.m_StencilBits;
				m_VSync = other.m_VSync;
				m_Fullscreen = other.m_Fullscreen;
			}
			return *this;
		}
	};

} // end namespace

