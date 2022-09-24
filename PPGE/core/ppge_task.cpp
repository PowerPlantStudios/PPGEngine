#include "ppge_task.h"

namespace PPGE
{
Task::Task() : m_future(m_signal.get_future())
{
}

Task::~Task()
{
    if (is_running())
        stop();
}

Task::Task(Task &&rhs) noexcept : m_signal(std::move(rhs.m_signal)), m_future(std::move(rhs.m_future))
{
}

Task &Task::operator=(Task &&rhs) noexcept
{
    m_signal = std::move(rhs.m_signal);
    m_future = std::move(rhs.m_future);
    return *this;
}

void Task::stop()
{
    m_signal.set_value();
}

void Task::operator()()
{
    run();
}

bool Task::is_running()
{
    if (m_future.wait_for(std::chrono::milliseconds(0)) == std::future_status::timeout)
        return true;
    return false;
}
} // namespace PPGE