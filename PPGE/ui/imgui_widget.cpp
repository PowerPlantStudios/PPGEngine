 #include "imgui_widget.h"

 #include "platform/d3d11/imgui_widget_d3d11.h"
 #include "platform/gl/imgui_widget_gl.h"
 #include "system/renderer_system.h"

 namespace PPGE
 {
 std::unique_ptr<ImGuiWidget> ImGuiWidget::CreateImGuiLayer()
 {
     auto api = RendererSystem::Get().GetRendererAPI();
     switch (api)
     {
     case PPGE::RendererAPI::OpenGL:
         return std::make_unique<ImGuiWidgetGL>();
     case PPGE::RendererAPI::Vulkan:
         return nullptr;
 #if defined(PPGE_PLATFORM_WIN)
     case PPGE::RendererAPI::D3D11:
         return std::make_unique<ImGuiWidgetD3D11>();
     case PPGE::RendererAPI::D3D12:
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
