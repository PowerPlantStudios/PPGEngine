#include "game_timer.h"

namespace PPGE
{
GameTimer::GameTimer() : m_check_point(std::chrono::steady_clock::now()), m_delta_time(0.0f), b_is_paused(false)
{
}

void GameTimer::Tick()
{
    if (b_is_paused)
        return;

    const auto now = std::chrono::steady_clock::now();
    m_delta_time = now - m_check_point;
    m_check_point = now;
}

void GameTimer::Pause()
{
    b_is_paused = true;
    m_delta_time = std::chrono::duration<float>::zero();
}

void GameTimer::Resume()
{
    b_is_paused = false;
    m_check_point = std::chrono::steady_clock::now();
}
} // namespace PPGE