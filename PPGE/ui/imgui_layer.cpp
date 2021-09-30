#include "PPGEpch.h"

#include "imgui_layer.h"

#if RENDERING_API_OPENGL
#include "platform/gl/imgui_layer_gl.h"
typedef PPGE::ImGuiLayerGL ImGuiLayerImpl;
#else

#endif

namespace PPGE
{
std::unique_ptr<ImGuiLayer> ImGuiLayer::CreateImGuiLayer()
{
    return std::make_unique<ImGuiLayerImpl>();
}
} // namespace PPGE
