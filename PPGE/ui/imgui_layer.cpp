#include "imgui_layer.h"

#include "platform/dx11/imgui_layer_dx11.h"
#include "platform/gl/imgui_layer_gl.h"
#include "system/renderer_system.h"

namespace PPGE
{
std::unique_ptr<ImGuiLayer> ImGuiLayer::CreateImGuiLayer()
{
    auto api = RendererSystem::Get().GetRendererAPI();
    switch (api)
    {
    case PPGE::RendererAPI::OpenGL:
        return std::make_unique<ImGuiLayerGL>();
    case PPGE::RendererAPI::Vulkan:
        return nullptr;
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::RendererAPI::DX11:
        return std::make_unique<ImGuiLayerDX11>();
    case PPGE::RendererAPI::DX12:
        return nullptr;
#endif
#if defined(PPGE_PLATFORM_APPLE)
    case PPGE::RendererAPI::Metal:
        return nullptr;
#endif
    case PPGE::RendererAPI::None:
        PPGE_ASSERT(false, "RendererAPI::None is not a valid option to create ImGui layer.");
        return nullptr;
    default:
        PPGE_ASSERT(false, "Given RendererAPI '{0}' is not valid.", static_cast<int>(api));
        return nullptr;
    }
}
} // namespace PPGE
