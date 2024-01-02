// include the needed header files
#include "KLS_Logger.h"
#include "KLS_String.h"
#include "KLS_Event.h"
#include "spdlog/fmt/ostr.h" // must be included

// unclutter the global namespace
namespace KLS
{
	// our own and only global logger instance
	KLS_Logger gGlobalLogger;
	KLS_API Ref<spdlog::logger> g_CoreLogger;
	Ref<spdlog::logger>& KLS_Logger::GetCoreLogger() { return g_CoreLogger; }

	KLS_Logger::KLS_Logger()
	{
		// setup the global logger
		initialize();

		// log this activity
		KLS_INFO("Logger initiated");
	}

	KLS_Logger::~KLS_Logger()
	{
		// log this activity
		KLS_INFO("Logger destroyed");
	}

	// setup our global logger
	void KLS_Logger::initialize()
	{
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("KLS.log", true));

		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		g_CoreLogger = std::make_shared<spdlog::logger>("KLS", begin(logSinks), end(logSinks));
		spdlog::register_logger(g_CoreLogger);
		g_CoreLogger->set_level(KLS_LOGLEVEL);
		g_CoreLogger->flush_on(KLS_LOGLEVEL);
	}

	// log an event to through the logger
	void KLS_Logger::LogEvent(const KLS_Event& e)
	{
		std::string text = "Event : ";
		text += toString(e.FromID);
		text += " ";
		text += e.FromName.c_str();
		text += " ";
		text += KLS_EVENTTYPE_STRING[e.EventType];
		padToLength(text, KLS_PAD_EVENTTYPE, ' ');

		switch (e.EventType)
		{
		case KLS_EVT_SYSTEM:
			if (KLS_LOGGING_FLAGS & KLS_LOGGING_FLAG_SYSTEM)
			{
				text += " ";
				text += KLS_EVENT_SYSTEM_STRING[e.System.EventType];
				padToLength(text, KLS_PAD_EVENTTYPE2, ' ');
				KLS_TRACE(text);
			} break;
		case KLS_EVT_KEYBOARD:
			if (KLS_LOGGING_FLAGS & KLS_LOGGING_FLAG_KEYBOARD)
			{
				text += " ";
				if (e.Keyboard.KeyDown) text += "Key Pressed";
				else text += "Key Released";
				padToLength(text, KLS_PAD_EVENTTYPE2, ' ');
				text += keyCodetoString(e.Keyboard.Key, e.Keyboard.Scancode);
				KLS_TRACE(text.c_str());
			} break;
		case KLS_EVT_MOUSE:
			if (KLS_LOGGING_FLAGS & KLS_LOGGING_FLAG_MOUSE)
			{
				if (e.Mouse.EventType == KLS_EVT_MOUSE_MOVED)
				{
					if (KLS_LOGGING_FLAGS & KLS_LOGGING_FLAG_MOUSEMOVE)
					{
						text += " ";
						text += KLS_EVENT_MOUSE_STRING[e.Mouse.EventType];
						padToLength(text, KLS_PAD_EVENTTYPE2, ' ');
						text += "X ";
						text += toString(e.Mouse.X);
						padToLength(text, KLS_PAD_X, ' ');
						text += "Y ";
						text += toString(e.Mouse.Y);
						padToLength(text, KLS_PAD_Y, ' ');
						KLS_TRACE(text);
					}
				}
				else
					if (e.Mouse.EventType == KLS_EVT_MOUSE_WHEEL)
					{
						text += " ";
						text += KLS_EVENT_MOUSE_STRING[e.Mouse.EventType];
						padToLength(text, KLS_PAD_EVENTTYPE2, ' ');
						text += "X ";
						text += toString(e.Mouse.X);
						padToLength(text, KLS_PAD_X, ' ');
						text += "Y ";
						text += toString(e.Mouse.Y);
						padToLength(text, KLS_PAD_Y, ' ');
						text += " Wheel ";
						text += toString(e.Mouse.Wheel);
						KLS_TRACE(text);
					}
					else
					{
						text += " ";
						text += KLS_EVENT_MOUSE_STRING[e.Mouse.EventType];
						padToLength(text, KLS_PAD_EVENTTYPE2, ' ');
						text += "X ";
						text += toString(e.Mouse.X);
						padToLength(text, KLS_PAD_X, ' ');
						text += "Y ";
						text += toString(e.Mouse.Y);
						padToLength(text, KLS_PAD_Y, ' ');
						KLS_TRACE(text);
					}
			} break;
		}
	}

} // end namespace


