// parse this file only once
#pragma once

// include the needed header files
#include "KLS_Application.h"

// unclutter the global namespace
namespace KLS
{
	/*
	*	derived from KLS_Application to provide functionality
	*/
	class myApp : public KLS_Application
	{
	private:

	public:
		// class constructor / destructor (use create / cleanup instead)
		myApp();
		virtual ~myApp();

		// dual creation allows for better error handling
		virtual bool create(KLS_ApplicationCreationInfo& info);
		
		// cleanup whatever memory mess we made
		virtual bool cleanup();

		// frame methods
		virtual void preFrame(const double& elapsedtime);
		virtual void frame(const double& elapsedtime);

		// event overrides to provide functionality
		virtual bool onKeyDown(const KLS_Event& e);

		// imgui related methods
		virtual void ImGui_draw();
	};


} // end namespace