// parse this file only once
#pragma once

// include the default header files
#include "_KLS_Defines.h"
#include "_KLS_Includes.h"
#include "_KLS_Enums.h"

// include the specific header files
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

// unclutter the global namespace
namespace KLS
{
	// forward declarations
	typedef struct KLS_Event KLS_Event;

	// easy method to create a shared_ptr
	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> KLS_API CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	// bascially a global logger
	// the static variables are treated as global variables
	class KLS_API KLS_Logger
	{
	public:
		// class constructor (calls initialize())
		KLS_Logger();

		// class destructor (cleans up memory mess)
		~KLS_Logger();

		// setup internal structures
		static void initialize();

		// log the event
		static void LogEvent(const KLS_Event& e);

		// return pointers to the loggers
		static Ref<spdlog::logger>& GetCoreLogger();
	private:
	};

	// instantiate our one and only global logger
	extern KLS_API Ref<spdlog::logger> g_CoreLogger;

} // end namespace


