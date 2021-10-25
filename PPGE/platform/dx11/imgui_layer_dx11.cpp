#include "imgui_layer_dx11.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <imgui.h>

#include "system/display_system.h"

namespace PPGE
{
void ImGuiLayerDX11::OnAttach()
{
}

void ImGuiLayerDX11::OnDetach()
{
}

void ImGuiLayerDX11::OnImGuiBegin()
{
}

void ImGuiLayerDX11::OnRender()
{
}

void ImGuiLayerDX11::OnInputEvent(InputEvent &event)
{
}

void ImGuiLayerDX11::OnApplicationEvent(ApplicationEvent &event)
{
    /* Maybe we can handle window size changed, application minimized or maximized events */
}
} // namespace PPGE
