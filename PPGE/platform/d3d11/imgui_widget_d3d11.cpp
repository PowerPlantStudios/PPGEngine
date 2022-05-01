#include "imgui_widget_d3d11.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>

#include "platform/d3d11/device_d3d11_impl.h"
#include "platform/d3d11/device_context_d3d11_impl.h"
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

    auto d3d11_device = static_cast<DeviceD3D11Impl *>(RendererSystem::Get().GetDevice())->GetD3D11Device();
    auto d3d11_immediate_context =
        static_cast<DeviceContextD3D11Impl *>(RendererSystem::Get().GetImmediateContext())->GetD3D11DeviceContext();

    ImGui_ImplDX11_Init(d3d11_device, d3d11_immediate_context);
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
    auto swap_chain = RendererSystem::Get().GetSwapChain();
    auto color_buffer_rtv = swap_chain->GetBackBufferRTV();
    auto depth_buffer_dsv = swap_chain->GetDepthBufferDSV();
    auto device_context = RendererSystem::Get().GetImmediateContext();
    device_context->SetRenderTargets(1, &color_buffer_rtv, depth_buffer_dsv);

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiWidgetD3D11::OnInputEvent(InputEvent &event)
{
     ImGuiIO &io = ImGui::GetIO();
     event.SetHandled(event.IsInCategory(InputEventCategoryBit::Mouse) && io.WantCaptureMouse);
     event.SetHandled(event.IsInCategory(InputEventCategoryBit::Keyboard) && io.WantCaptureKeyboard);
}

void ImGuiWidgetD3D11::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE
