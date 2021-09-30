#include "PPGEpch.h"

#include "display_system.h"

#if defined(DISPLAY_API_GLFW)
#include "platform/glfw/display_system_glfw.h"
typedef PPGE::DisplaySystemGLFW DisplaySystemImp;
#elif defined(DISPLAY_API_WIN32)
// Include corresponding header file to start win32 api window
#endif

namespace PPGE
{

DisplaySystem *DisplaySystem::s_instance = new DisplaySystemImp();

} // namespace PPGE
