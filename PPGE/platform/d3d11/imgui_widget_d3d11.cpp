#include "imgui_widget_d3d11.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "platform/d3d11/renderer_system_d3d11.h"
#include "system/display_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
void ImGuiWidgetD3D11::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;   // Enable Multi-Viewport / Platform Windows

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    HWND window_handle = static_cast<HWND>(DisplaySystem::Get().GetNativeDisplayPtr());
    ImGui_ImplWin32_Init(window_handle);

    m_renderer = RendererSystem::GetRendererSystem<RendererSystemD3D11>();
    ImGui_ImplDX11_Init(m_renderer->m_device, m_renderer->m_immediate_context);
}

void ImGuiWidgetD3D11::OnDetach()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiWidgetD3D11::ImGuiBegin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiWidgetD3D11::ImGuiEnd()
{
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }

    m_renderer->m_immediate_context->OMSetRenderTargets(1, &(m_renderer->m_render_target_view),
                                                        m_renderer->m_depth_stencil_view);
}

void ImGuiWidgetD3D11::OnInputEvent(InputEvent &event)
{
    // ImGuiIO &io = ImGui::GetIO();
    // event.SetHandled(event.IsInCategory(InputEventCategoryBit::Mouse) & io.WantCaptureMouse);
    // event.SetHandled(event.IsInCategory(InputEventCategoryBit::Keyboard) & io.WantCaptureKeyboard);
}

void ImGuiWidgetD3D11::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE
