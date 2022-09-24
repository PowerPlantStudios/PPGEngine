#include "file_system_observer.h"

#include "system/logger_system.h"

namespace PPGE
{
void FileSystemObserver::StartObserving()
{
    if (m_thread.joinable())
    {
        PPGE_WARN("Another thread for file system observer is alread running. It will be stopped first.");
        StopObserving();
    }
    PPGE_ASSERT(!m_thread.joinable(), "Another thread for filesystem is still running.");

    if (!std::filesystem::exists(m_path))
    {
        PPGE_ASSERT(false,
                    "File system observer path doesn't exist. Set the path before start polling file system actions.");
        return;
    }

    if (!m_callback)
    {
        PPGE_ASSERT(false,
                    "File system observer hasn't been set. Set the callback before start polling file system actions.");
        return;
    }

    m_thread = std::thread([this]() { m_observer_task.run(); });
}

void FileSystemObserver::StopObserving()
{
    m_observer_task.stop();
    m_thread.join();
    m_thread = std::thread();
}
} // namespace PPGE