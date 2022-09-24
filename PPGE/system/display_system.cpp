#include "display_system.h"

#include "platform/glfw/display_system_glfw.h"
#if defined(PPGE_PLATFORM_WIN)
#include "platform/win32/display_system_win32.h"
#endif

namespace PPGE
{
DisplaySystem *DisplaySystem::s_instance = nullptr;

void DisplaySystem::Initialize(WindowAPIType api)
{
    switch (api)
    {
    case PPGE::WindowAPIType::GLFW:
        s_instance = new DisplaySystemGLFW();
        break;
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::WindowAPIType::Win32:
        s_instance = new DisplaySystemWin32();
        break;
#endif
    case PPGE::WindowAPIType::None:
        PPGE_ASSERT(false, "WindowAPI::None is not a valid option for DisplaySystem initialization.");
        break;
    default:
        PPGE_ASSERT(false, "Given WindowAPI '{0}' is not valid.", static_cast<int>(api));
    }
}
} // namespace PPGE
