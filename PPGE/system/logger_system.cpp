#include "logger_system.h"

namespace PPGE
{
LoggerSystem *LoggerSystem::s_instance = new LoggerSystem();
std::shared_ptr<spdlog::logger> LoggerSystem::s_PPGE_logger;
std::shared_ptr<spdlog::logger> LoggerSystem::s_client_logger;

void LoggerSystem::StartUp(const LoggerSystemProps &props)
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("PPGE.log", true));

    // Default pattern for all logger instances
    sinks[0]->set_pattern("%^[%T] %n: %v%$");
    sinks[1]->set_pattern("[%T] [%l] %n: %v");

    // Create engine logger and set default logging level
    s_PPGE_logger = std::make_shared<spdlog::logger>("PPGE", sinks.begin(), sinks.end());
    spdlog::register_logger(s_PPGE_logger);
    s_PPGE_logger->set_level(spdlog::level::trace);
    s_PPGE_logger->flush_on(spdlog::level::trace);

    // Create client logger and set default logging level
    s_client_logger = std::make_shared<spdlog::logger>("APPLICATION", sinks.begin(), sinks.end());
    spdlog::register_logger(s_client_logger);
    s_client_logger->set_level(spdlog::level::trace);
    s_client_logger->flush_on(spdlog::level::trace);
}

void LoggerSystem::Update()
{
}

void LoggerSystem::ShutDown()
{
}

} // namespace PPGE
