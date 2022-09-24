#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "io/file_system_observer.h"

namespace PPGE
{

class FileSystemObserverWindows final : public FileSystemObserver
{
  public:
    FileSystemObserverWindows();

    ~FileSystemObserverWindows();

    void SetPath(const std::filesystem::path &path) override final;

  protected:
    void PollFilesystemEvents() override final;

  private:
    HANDLE m_handle;
    OVERLAPPED m_overlapped_handle;
};
} // namespace PPGE