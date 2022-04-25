#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

// WARNING
// https://docs.microsoft.com/en-us/cpp/standard-library/chrono
// According to statement in the MS documentation chrono library
// for Windows uses QueryPerformanceCounter. Be careful for that
// in the future. Since there are so many complaints related
// to QueryPerformanceCounter for multi-threaded applications.

namespace PPGE
{
class GameTimer
{
  public:
    GameTimer();

    ~GameTimer() = default;

    void Tick();
    void Pause();
    void Resume();

    inline float GetDeltaTime() const
    {
        return m_delta_time.count(); 
    }


  private:
    std::chrono::steady_clock::time_point m_check_point;
    std::chrono::duration<float> m_delta_time;
    bool b_is_paused;
};
} // namespace PPGE