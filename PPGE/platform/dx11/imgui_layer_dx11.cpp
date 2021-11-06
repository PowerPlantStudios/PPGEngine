#include "imgui_layer_dx11.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

#include "platform/dx11/renderer_system_dx11.h"
#include "system/display_system.h"
#include "system/renderer_system.h"

namespace PPGE
{
void ImGuiLayerDX11::OnAttach()
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
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

    RendererSystemDX11 *renderer = RendererSystem::GetRenderer<RendererSystemDX11>();
    ImGui_ImplDX11_Init(renderer->GetDeice(), renderer->GetImmediateContext());

    ImGui_ImplWin32_EnableDpiAwareness();
}

void ImGuiLayerDX11::OnDetach()
{
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiLayerDX11::OnImGuiBegin()
{
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayerDX11::OnRender()
{
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)DisplaySystem::Get().GetWidth(), (float)DisplaySystem::Get().GetHeight());

    // Rendering
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    RendererSystemDX11 *renderer = RendererSystem::GetRenderer<RendererSystemDX11>();
    ID3D11RenderTargetView *render_target_view = renderer->GetRenderTargetView();
    ID3D11DepthStencilView *depth_stencil_view = renderer->GetDepthStencilView();
    renderer->GetImmediateContext()->OMSetRenderTargets(1, &render_target_view, depth_stencil_view);
    renderer->GetImmediateContext()->ClearRenderTargetView(render_target_view,
                                                           PPGE::Math::Color(0.15f, 0.15f, 0.15f));
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiLayerDX11::OnInputEvent(InputEvent &event)
{
    ImGuiIO &io = ImGui::GetIO();
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Mouse) & io.WantCaptureMouse);
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Keyboard) & io.WantCaptureKeyboard);
}

void ImGuiLayerDX11::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE
