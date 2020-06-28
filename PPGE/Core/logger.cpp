#include "logger.h"

namespace PPGE
{

	Shared<spdlog::logger> Logger::m_PPGELogger;
	Shared<spdlog::logger> Logger::m_ClientLogger;

	void Logger::Initialize()
	{
		// Default pattern for all logger instances
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		// Create engine logger and set default logging level
		m_PPGELogger = spdlog::stderr_color_mt("PPGE");
		m_PPGELogger->set_level(spdlog::level::trace);

		// Create client logger and set default logging level
		m_ClientLogger = spdlog::stderr_color_mt("APPLICATION");
		m_ClientLogger->set_level(spdlog::level::trace);
	}

}
