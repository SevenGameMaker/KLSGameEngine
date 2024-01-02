// parse this file only once
#pragma once

// include the needed header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// unclutter the global namespace
namespace KLS
{
	/*
	* a simple timer class
	*/
	class KLS_API KLS_Timer
	{
	private:
		// variables for tracking time
		std::chrono::high_resolution_clock::time_point m_PreviousTime;
		std::chrono::high_resolution_clock::time_point m_CurrentTime;

		// variables for tracking framerate
		KLS_VARIABLE(int, NumberOfFrames, 0);
		KLS_VARIABLE_GET(double, FPS, 0);
		KLS_VARIABLE(bool, Paused, false);

		// variables
		KLS_VARIABLE_GET(double, Time, 0);
		KLS_VARIABLE_GET(double, TotalTime, 0);
		KLS_VARIABLE_GET(double, DeltaTime, 0.0);
		KLS_VARIABLE_GET(double, FPSTime, 0);

	public:
		// class constructor / destructor
		KLS_Timer();
		virtual ~KLS_Timer();

		// current framerate
		double getFrameRate() const;

		// frame method
		double tick();

		// pause the timer
		void pause();

		// resume the timer
		void resume();

		// helper methods
		std::string getTotalTimeString();
		std::string getDeltaTimeString();
		std::string getFrameRateString();
	};

} // end namespace