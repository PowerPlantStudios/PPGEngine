#include "PPGEpch.h"

#include "display_system.h"

#if DISPLAY_API_GLFW
#include "platform/glfw/display_system_glfw.h"

typedef PPGE::DisplaySystemGLFW DisplaySystemImp;

#elif DISPLAY_API_WIN32
// Include corresponding header file to start win32 api window
#endif

namespace PPGE
{

DisplaySystem &DisplaySystem::s_instance = DisplaySystemImp();

} // namespace PPGE
