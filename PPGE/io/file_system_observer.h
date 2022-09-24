#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_task.h"

namespace PPGE
{

class FileSystemObserver
{
  public:
    enum class FileAction
    {
        UNKNOWN = 0,
        ADDED,
        MODIFIED,
        REMOVED,
        RENAMED_FROM,
        RENAMED_TO
    };

    using Callback = std::function<void(const std::filesystem::path &, FileAction)>;

    FileSystemObserver() : m_observer_task(*this), m_thread{}, m_path{}, m_callback{}
    {
    }

    virtual ~FileSystemObserver()
    {
        if (m_thread.joinable())
            StopObserving();
    }

    virtual void SetPath(const std::filesystem::path &path)
    {
        m_path = path;
    }

    virtual void SetCallback(const Callback &callback)
    {
        m_callback = callback;
    }

    void StartObserving();

    void StopObserving();

  protected:
    virtual void PollFilesystemEvents() = 0;

    std::filesystem::path m_path;
    Callback m_callback;

  private:
    class ObserverTask : public Task
    {
      public:
        ObserverTask(FileSystemObserver &observer_ref) : Task(), m_observer_ref(observer_ref)
        {
        }

        void run() override
        {
            while (is_running())
                m_observer_ref.PollFilesystemEvents();
        }

      private:
        FileSystemObserver &m_observer_ref;
    };

    ObserverTask m_observer_task;
    std::thread m_thread;
};
} // namespace PPGE