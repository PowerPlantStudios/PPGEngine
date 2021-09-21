#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/smart_ptr.h"
#include "systems/isystem.h"

namespace PPGE
{
struct LoggerSystemProps
{
};

class PPGE_API LoggerSystem : public ISystem<LoggerSystemProps>
{
  public:
    LoggerSystem()
    {
    }
    ~LoggerSystem()
    {
    }

    virtual void StartUp(const LoggerSystemProps &props) override;
    virtual void Update() override;
    virtual void ShutDown() override;

    inline static Shared<spdlog::logger> &GetPPGELogger()
    {
        return s_PPGELogger;
    }
    inline static Shared<spdlog::logger> &GetClientLogger()
    {
        return s_ClientLogger;
    }

    inline static LoggerSystem &GetLoggerSystem()
    {
        return s_instance;
    }

  private:
    static LoggerSystem &s_instance;
    static Shared<spdlog::logger> s_PPGELogger;
    static Shared<spdlog::logger> s_ClientLogger;
};

} // namespace PPGE

// Macros for convenience
// For distribution build they will be stripped out
// TODO: Add build type check to strip away logging for distribution builds.
#define PPGE_TRACE(...) PPGE::LoggerSystem::GetPPGELogger()->trace(__VA_ARGS__)
#define PPGE_INFO(...) PPGE::LoggerSystem::GetPPGELogger()->info(__VA_ARGS__)
#define PPGE_WARN(...) PPGE::LoggerSystem::GetPPGELogger()->warn(__VA_ARGS__)
#define PPGE_ERROR(...) PPGE::LoggerSystem::GetPPGELogger()->error(__VA_ARGS__)
#define PPGE_CRITICAL(...) PPGE::LoggerSystem::GetPPGELogger()->critical(__VA_ARGS__)
#define APP_TRACE(...) PPGE::LoggerSystem::GetClientLogger()->trace(__VA_ARGS__)
#define APP_INFO(...) PPGE::LoggerSystem::GetClientLogger()->info(__VA_ARGS__)
#define APP_WARN(...) PPGE::LoggerSystem::GetClientLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...) PPGE::LoggerSystem::GetClientLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...) PPGE::LoggerSystem::GetClientLogger()->critical(__VA_ARGS__)
