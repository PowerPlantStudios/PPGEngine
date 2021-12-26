#include "imgui_widget_d3d11.h"

#include <backends/imgui_impl_dx11.h>
#include <backends/imgui_impl_win32.h>
#include <imgui.h>

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

    RendererSystemD3D11 *renderer = RendererSystem::GetRendererSystem<RendererSystemD3D11>();
    ImGui_ImplDX11_Init(renderer->m_device, renderer->m_immediate_context);

    ImGui_ImplWin32_EnableDpiAwareness();
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
    ImGuiIO &io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)DisplaySystem::Get().GetWidth(), (float)DisplaySystem::Get().GetHeight());

    // Rendering
    bool show_demo_window = true;
    ImGui::ShowDemoWindow(&show_demo_window);
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }
}

void ImGuiWidgetD3D11::OnInputEvent(InputEvent &event)
{
    ImGuiIO &io = ImGui::GetIO();
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Mouse) & io.WantCaptureMouse);
    event.SetHandled(event.IsInCategory(InputEventCategoryBit::Keyboard) & io.WantCaptureKeyboard);
}

void ImGuiWidgetD3D11::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE