#include "renderer_system.h"

#include "platform/gl/renderer_system_gl.h"
#if defined(PPGE_PLATFORM_WIN)
#include "platform/dx11/renderer_system_dx11.h"
#endif
#if defined(PPGE_PLATFORM_APPLE)
#include "platform/dx11/renderer_system_mtl.h"
#endif

namespace PPGE
{
RendererSystem *RendererSystem::s_instance = nullptr;

void RendererSystem::Initialize(RendererAPI api)
{
    switch (api)
    {
    case PPGE::RendererAPI::OpenGL:
        s_instance = new RendererSystemGL();
        break;
    case PPGE::RendererAPI::Vulkan:
        break;
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::RendererAPI::DX11:
        s_instance = new RendererSystemDX11();
        break;
    case PPGE::RendererAPI::DX12:
        break;
#endif
#if defined(PPGE_PLATFORM_APPLE)
    case PPGE::RendererAPI::Metal:
        break;
#endif
    case PPGE::RendererAPI::None:
        PPGE_ASSERT(false, "RendererAPI::None is not a valid option for RendererSystem initialization.");
        break;
    default:
        PPGE_ASSERT(false, "Given RendererAPI '{0}' is not valid.", static_cast<int>(api));
    }
}
} // namespace PPGE
