#include "renderer_system.h"

#include "platform/gl/renderer_system_gl.h"
#if defined(PPGE_PLATFORM_WIN)
#include "platform/d3d11/renderer_system_d3d11.h"
#endif
#if defined(PPGE_PLATFORM_APPLE)
#include "platform/metal/renderer_system_metal.h"
#endif

namespace PPGE
{
RendererSystem *RendererSystem::s_instance = nullptr;

void RendererSystem::Initialize(RendererAPIType api)
{
    PPGE_ASSERT(IsRendererAPISupported(api), "Renderer API is not suppoerted on the current platform.");
    switch (api)
    {
    case PPGE::RendererAPIType::OpenGL:
        s_instance = new RendererSystemGL();
        break;
    case PPGE::RendererAPIType::Vulkan:
        break;
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::RendererAPIType::D3D11:
        s_instance = new RendererSystemD3D11();
        break;
    case PPGE::RendererAPIType::D3D12:
        break;
#endif
#if defined(PPGE_PLATFORM_APPLE)
    case PPGE::RendererAPIType::Metal:
        break;
#endif
    case PPGE::RendererAPIType::None:
        PPGE_ASSERT(false, "RendererAPI::None is not a valid option for RendererSystem initialization.");
        break;
    default:
        PPGE_ASSERT(false, "Given RendererAPI '{0}' is not valid.", static_cast<int>(api));
    }
}
} // namespace PPGE
