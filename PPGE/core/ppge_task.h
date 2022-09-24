#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
class PPGE_API Task
{
  public:
    Task();

    ~Task();

    Task(const Task &) = delete;

    Task(Task &&rhs) noexcept;

    Task &operator=(const Task &rhs) = delete;

    Task &operator=(Task &&rhs) noexcept;

    virtual void run() = 0;

    void stop();

    void operator()();

  protected:
    bool is_running();

  private:
    std::promise<void> m_signal;
    std::future<void> m_future;
};
} // namespace PPGE