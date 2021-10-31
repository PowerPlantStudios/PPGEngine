#include "imgui_layer.h"

#include "platform/gl/imgui_layer_gl.h"
#include "platform/dx11/imgui_layer_dx11.h"

namespace PPGE
{
std::unique_ptr<ImGuiLayer> ImGuiLayer::CreateImGuiLayer()
{
    return std::make_unique<ImGuiLayerGL>();
}
} // namespace PPGE
