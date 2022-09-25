#include "file_system_observer_windows.h"

#include "system/logger_system.h"

std::string ws2s(const std::wstring &wstr)
{
    static std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}

namespace PPGE
{
static uint8_t s_change_buf[1024];
static DWORD s_change_buf_size = sizeof(s_change_buf) / sizeof(uint8_t);

FileSystemObserverWindows::FileSystemObserverWindows()
    : FileSystemObserver(), m_handle{INVALID_HANDLE_VALUE}, m_overlapped_handle{0}
{
    m_overlapped_handle.hEvent = CreateEventW(NULL, FALSE, 0, NULL);
    PPGE_ASSERT(m_overlapped_handle.hEvent, "Failed to create event");
}

FileSystemObserverWindows::~FileSystemObserverWindows()
{
    if (m_handle != INVALID_HANDLE_VALUE)
        CloseHandle(m_handle);
}

void FileSystemObserverWindows::SetPath(const std::filesystem::path &path)
{
    FileSystemObserver::SetPath(path);

    if (m_handle != INVALID_HANDLE_VALUE)
        CloseHandle(m_handle);

    m_handle = CreateFileW(m_path.c_str(), FILE_LIST_DIRECTORY, FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE,
                           NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED, NULL);
    PPGE_ASSERT(m_handle != INVALID_HANDLE_VALUE, "Opening a file handle has failed.");

    BOOL success = ReadDirectoryChangesW(m_handle, s_change_buf, s_change_buf_size, TRUE,
                                         FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME |
                                             FILE_NOTIFY_CHANGE_DIR_NAME,
                                         NULL, &m_overlapped_handle, NULL);
    PPGE_ASSERT(success, "Call to ReadDirectoryChangesW has failed.");
}

void FileSystemObserverWindows::PollFilesystemEvents()
{
    DWORD result = WaitForSingleObject(m_overlapped_handle.hEvent, 100);
    switch (result)
    {
    case WAIT_OBJECT_0: {
        DWORD bytes_transferred;
        GetOverlappedResult(m_handle, &m_overlapped_handle, &bytes_transferred, FALSE);
        PPGE_ASSERT(bytes_transferred < s_change_buf_size, "Change buffer size is less than transferred data size.")

        DWORD offset = 0;
        FILE_NOTIFY_INFORMATION *file_notify_info = (FILE_NOTIFY_INFORMATION *)s_change_buf;
        do
        {
            *((uint8_t **)&file_notify_info) += offset;

            std::filesystem::path file_name =
                m_path / std::wstring(file_notify_info->FileName, file_notify_info->FileNameLength / sizeof(wchar_t));
            file_name = std::filesystem::weakly_canonical(file_name);

            switch (file_notify_info->Action)
            {
            case FILE_ACTION_ADDED: {
                m_callback(file_name, FileAction::ADDED);
            }
            break;
            case FILE_ACTION_REMOVED: {
                m_callback(file_name, FileAction::REMOVED);
            }
            break;
            case FILE_ACTION_MODIFIED: {
                m_callback(file_name, FileAction::MODIFIED);
            }
            break;
            case FILE_ACTION_RENAMED_OLD_NAME: {
                m_callback(file_name, FileAction::RENAMED_FROM);
            }
            break;
            case FILE_ACTION_RENAMED_NEW_NAME: {
                m_callback(file_name, FileAction::RENAMED_TO);
            }
            break;
            default: {
                PPGE_ERROR("FILE_NOTIFY_INFORMATION.Action '{0}' is unknown action!", file_notify_info->Action);
                PPGE_ASSERT(false, "Unknown file notify information action.");
            }
            break;
            }

            offset = file_notify_info->NextEntryOffset;
        } while (file_notify_info->NextEntryOffset);

        BOOL success = ReadDirectoryChangesW(m_handle, s_change_buf, s_change_buf_size, TRUE,
                                             FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_FILE_NAME |
                                                 FILE_NOTIFY_CHANGE_DIR_NAME,
                                             NULL, &m_overlapped_handle, NULL);
        PPGE_ASSERT(success, "Call to ReadDirectoryChangesW has failed.");
    }
    break;
    case WAIT_TIMEOUT: {
    }
    break;
    case WAIT_ABANDONED: {
        PPGE_ERROR("The specified object is a mutex object that was not released by the thread that owned the mutex "
                   "object before the owning thread terminated. Ownership of the mutex object is granted to the "
                   "calling thread and the mutex state is set to nonsignaled. If the mutex was protecting persistent "
                   "state information, you should check it for consistency.");
        PPGE_ASSERT(false, "WaitForSingleObject returned WAIT_ABANDONED.");
    }
    break;
    case WAIT_FAILED: {
        PPGE_ERROR("Call to WaitForSingleObject has failed.");
        PPGE_ASSERT(false, "WaitForSingleObject returned WAIT_FAILED.");
    }
    break;
    default: {
        PPGE_ERROR("Call to WaitForSingleObject has returned unknown result.");
        PPGE_ASSERT(false, "Unexpected error happened in FileSystemObserverWindows::PollFilesystemEvents.");
    }
    break;
    }

    return;
}
} // namespace PPGE
