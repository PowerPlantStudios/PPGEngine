#include "io_system.h"

#if defined(PPGE_PLATFORM_WIN)
#include "platform/windows/io_system_windows.h"
#elif defined(PPGE_PLATFORM_UNIX)
#elif defined(PPGE_PLATFORM_APPLE)
#else
#endif

namespace PPGE
{
IOSystem *IOSystem::s_instance = nullptr;

void IOSystem::Initialize(OSType api)
{
    switch (api)
    {
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::OSType::Windows:
        s_instance = new IOSystemWindows();
        break;
#elif defined(PPGE_PLATFORM_UNIX)
    case PPGE::OSType::Linux:
        break;
#elif defined(PPGE_PLATFORM_APPLE)
    case PPGE::OSType::Mac:
        break;
#endif
    case PPGE::OSType::Unknown:
        PPGE_ASSERT(false, "IOSystem cannot be initialized with OSType::Unknown.");
        break;
    default:
        PPGE_ASSERT(false, "Unknown OSType is passed.")
        break;
    }
}
} // namespace PPGE