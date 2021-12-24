#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/isystem.h"

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
    virtual void ShutDown() override;

    inline std::shared_ptr<spdlog::logger> &GetPPGELogger()
    {
        return s_PPGE_logger;
    }
    inline std::shared_ptr<spdlog::logger> &GetClientLogger()
    {
        return s_client_logger;
    }

  private:
    std::shared_ptr<spdlog::logger> s_PPGE_logger;
    std::shared_ptr<spdlog::logger> s_client_logger;

  public:
    static void Initialize();

    inline static void Destroy()
    {
        SAFE_DELETE(s_instance)
    }

    inline static LoggerSystem &Get()
    {
        return *s_instance;
    }
  private:
    static LoggerSystem *s_instance;
};

} // namespace PPGE

// Macros for convenience
// For distribution build they will be stripped out
// TODO: Add build type check to strip away logging for distribution builds.
#define PPGE_TRACE(...) PPGE::LoggerSystem::Get().GetPPGELogger()->trace(__VA_ARGS__)
#define PPGE_INFO(...) PPGE::LoggerSystem::Get().GetPPGELogger()->info(__VA_ARGS__)
#define PPGE_WARN(...) PPGE::LoggerSystem::Get().GetPPGELogger()->warn(__VA_ARGS__)
#define PPGE_ERROR(...) PPGE::LoggerSystem::Get().GetPPGELogger()->error(__VA_ARGS__)
#define PPGE_CRITICAL(...) PPGE::LoggerSystem::Get().GetPPGELogger()->critical(__VA_ARGS__)
#define APP_TRACE(...) PPGE::LoggerSystem::Get().GetClientLogger()->trace(__VA_ARGS__)
#define APP_INFO(...) PPGE::LoggerSystem::Get().GetClientLogger()->info(__VA_ARGS__)
#define APP_WARN(...) PPGE::LoggerSystem::Get().GetClientLogger()->warn(__VA_ARGS__)
#define APP_ERROR(...) PPGE::LoggerSystem::Get().GetClientLogger()->error(__VA_ARGS__)
#define APP_CRITICAL(...) PPGE::LoggerSystem::Get().GetClientLogger()->critical(__VA_ARGS__)
