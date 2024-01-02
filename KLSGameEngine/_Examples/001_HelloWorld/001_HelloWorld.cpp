// 001_HelloWorld.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "myApp.h"
#include "KLS_ApplicationCreationInfo.h"

int main()
{
	KLS::KLS_ApplicationCreationInfo info;
		info.m_Title			= "KLSGameEngine";					// window titlebar text
		info.m_DriverType		= KLSDT_OPENGL;						// driver type
		info.m_WindowPosition	= glm::uvec4(0, 26, 1024, 768);		// initial window position / size
		info.m_Fullscreen		= false;							// fullscreen
		info.m_DecoratedWindow	= !info.m_Fullscreen;				// window style
		info.m_Resizeable		= true;								// is the window resizeable
		info.m_MultiSamples		= 0;								// multisample 
		info.m_DoubleBuffer		= true;								// doublebuffer
		info.m_DepthBits		= 16;								// depth bits
		info.m_StencilBits		= 16;								// stencil bits
		info.m_VSync			= KLSVS_UNLIMITED;					// vsynx

	// create an instnace of teh myApp class and let it run
	KLS::myApp* app = new KLS::myApp();
		if (app->create(info)) app->run();
		app->cleanup();
		delete(app);
	return 0;
}

