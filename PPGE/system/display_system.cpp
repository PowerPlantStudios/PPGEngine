#include "display_system.h"

#if defined(DISPLAY_API_GLFW)
#include "platform/glfw/display_system_glfw.h"
typedef PPGE::DisplaySystemGLFW DisplaySystemImp;
#elif defined(DISPLAY_API_WIN32)
#include "platform/win32/display_system_win32.h"
typedef PPGE::DisplaySystemWin32 DisplaySystemImp;
#endif

namespace PPGE
{

DisplaySystem *DisplaySystem::s_instance = new DisplaySystemImp();

} // namespace PPGE
