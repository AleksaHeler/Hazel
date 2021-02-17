#include "hzpch.h"
#include "Log.h"
// Defining a Log class which will serve as a wrapper for spdlog api

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {

	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		// Color it, timestamp, name of the logger and then log message
		spdlog::set_pattern("%^[%T] %n: %v%$");

		// Set name and level
		s_CoreLogger = spdlog::stdout_color_mt("Hazel");
		s_CoreLogger->set_level(spdlog::level::trace);
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}

}
