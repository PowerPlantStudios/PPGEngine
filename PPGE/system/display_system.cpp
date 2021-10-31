#include "display_system.h"

#include "platform/glfw/display_system_glfw.h"
#include "platform/win32/display_system_win32.h"

namespace PPGE
{
DisplaySystem *DisplaySystem::s_instance = nullptr;

void DisplaySystem::Initialize(WindowAPI api)
{
    switch (api)
    {
    case PPGE::WindowAPI::GLFW:
        s_instance = new DisplaySystemGLFW();
        break;
    case PPGE::WindowAPI::Win32:
        s_instance = new DisplaySystemWin32();
        break;
    case PPGE::WindowAPI::None:
        PPGE_ASSERT(false, "WindowAPI::None is not a valid option for DisplaySystem initialization.");
        break;
    default:
        PPGE_ASSERT(false, "Given WindowAPI '{0}' is not recognized.", static_cast<int>(api));
    }
}
} // namespace PPGE
