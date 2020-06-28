#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "defines.h"
#include "smart_ptr.h"

namespace PPGE
{

	class _PPGE_API Logger
	{
	public:
		static void Initialize();

		inline static Shared<spdlog::logger>& GetPPGELogger() { return m_PPGELogger; }
		inline static Shared<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

	private:
		static Shared<spdlog::logger> m_PPGELogger;
		static Shared<spdlog::logger> m_ClientLogger;
	};

}

// Macros for convenience
// For distribution build they will be stripped out
// TODO: Add build type check to strip away logging for distribution builds.
#define PPGE_TRACE(...)    PPGE::Logger::GetPPGELogger()->trace(__VA_ARGS__)
#define PPGE_INFO(...)     PPGE::Logger::GetPPGELogger()->info(__VA_ARGS__)
#define PPGE_WARN(...)     PPGE::Logger::GetPPGELogger()->warn(__VA_ARGS__)
#define PPGE_ERROR(...)    PPGE::Logger::GetPPGELogger()->error(__VA_ARGS__)
#define PPGE_CRITICAL(...) PPGE::Logger::GetPPGELogger()->critical(__VA_ARGS__)
#define APP_TRACE(...)     PPGE::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define APP_INFO(...)      PPGE::Logger::GetClientLogger()->info(__VA_ARGS__)
#define APP_WARN(...)      PPGE::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...)     PPGE::Logger::GetClientLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...)  PPGE::Logger::GetClientLogger()->critical(__VA_ARGS__)

