#include "imgui_widget.h"

#include "core/os_helper.h"
#include "platform/d3d11/imgui_widget_d3d11.h"
#include "platform/gl/imgui_widget_gl.h"
#include "system/renderer_system.h"

namespace PPGE
{
std::unique_ptr<ImGuiWidget> ImGuiWidget::CreateImGuiWidget()
{
    auto api = RendererSystem::Get().GetRendererAPI();
    switch (api)
    {
    case PPGE::RendererAPIType::OpenGL:
        return std::make_unique<ImGuiWidgetGL>();
    case PPGE::RendererAPIType::Vulkan:
        return nullptr;
#if defined(PPGE_PLATFORM_WIN)
    case PPGE::RendererAPIType::D3D11:
        return std::make_unique<ImGuiWidgetD3D11>();
    case PPGE::RendererAPIType::D3D12:
        return nullptr;
#endif
#if defined(PPGE_PLATFORM_APPLE)
    case PPGE::RendererAPIType::Metal:
        return nullptr;
#endif
    case PPGE::RendererAPIType::None:
        PPGE_ASSERT(false, "RendererAPI::None is not a valid option to create ImGui layer.");
        return nullptr;
    default:
        PPGE_ASSERT(false, "Given RendererAPI '{0}' is not valid.", static_cast<int>(api));
        return nullptr;
    }
}
} // namespace PPGE
