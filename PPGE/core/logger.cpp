#include "PPGEpch.h"

#include "logger.h"

namespace PPGE
{

	Shared<spdlog::logger> Logger::s_PPGELogger;
	Shared<spdlog::logger> Logger::s_ClientLogger;

	void Logger::Initialize()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.emplace_back(CreateShared<spdlog::sinks::stdout_color_sink_mt>());
		sinks.emplace_back(CreateShared<spdlog::sinks::basic_file_sink_mt>("PPGE.log", true));
		
		// Default pattern for all logger instances
		sinks[0]->set_pattern("%^[%T] %n: %v%$");
		sinks[1]->set_pattern("[%T] [%l] %n: %v");

		// Create engine logger and set default logging level
		s_PPGELogger = CreateShared<spdlog::logger>("PPGE", sinks.begin(), sinks.end());
		spdlog::register_logger(s_PPGELogger);
		s_PPGELogger->set_level(spdlog::level::trace);
		s_PPGELogger->flush_on(spdlog::level::trace);
		//s_PPGELogger->set_pattern("%^[%T] %n: %v%$");

		// Create client logger and set default logging level
		s_ClientLogger = CreateShared<spdlog::logger>("APPLICATION", sinks.begin(), sinks.end());
		spdlog::register_logger(s_ClientLogger);
		s_ClientLogger->set_level(spdlog::level::trace);
		s_ClientLogger->flush_on(spdlog::level::trace);
		//s_ClientLogger->set_pattern("[%T] [%l] %n: %v");
	}

}
