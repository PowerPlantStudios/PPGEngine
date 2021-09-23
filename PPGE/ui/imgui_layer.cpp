#include "PPGEpch.h"

#include "imgui_layer.h"

#if RENDERING_API_OPENGL
#include "platform/gl/imgui_layer_gl.h"
typedef PPGE::ImGuiLayerGL ImGuiLayerImpl;
#else

#endif

namespace PPGE
{
ImGuiLayer *ImGuiLayer::CreateImGuiLayer()
{
    return new ImGuiLayerImpl();
}
} // namespace PPGE
