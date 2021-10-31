#include "renderer_system.h"

namespace PPGE
{
RendererSystem *RendererSystem::s_instance = nullptr;

void RendererSystem::Initialize(RendererAPI api)
{
    switch (api)
    {
    case PPGE::RendererAPI::None:
        break;
    case PPGE::RendererAPI::OpenGL:
        break;
    case PPGE::RendererAPI::Vulkan:
        break;
    case PPGE::RendererAPI::DX11:
        break;
    case PPGE::RendererAPI::DX12:
        break;
    case PPGE::RendererAPI::Metal:
        break;
    default:
        break;
    }
}
} // namespace PPGE
