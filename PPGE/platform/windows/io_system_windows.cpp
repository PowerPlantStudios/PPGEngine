#include "io_system_windows.h"

#include "platform/windows/file_system_observer_windows.h"

namespace PPGE
{
std::unique_ptr<FileSystemObserver> PPGE::IOSystemWindows::GetFileSystemObserver()
{
    return std::make_unique<FileSystemObserverWindows>();
}
}
