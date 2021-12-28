#pragma once
#include "PPGEpch.h"

#include "ui/imgui_widget.h"

namespace PPGE
{
class RendererSystemD3D11;

class PPGE_API ImGuiWidgetD3D11 : public ImGuiWidget
{
  public:
    ImGuiWidgetD3D11() : ImGuiWidget("ImGui Layer (D3D11)")
    {
    }

    void OnAttach() override;

    void OnDetach() override;

    void ImGuiBegin() override;

    void ImGuiEnd() override;

    void OnInputEvent(InputEvent &event) override;

    void OnApplicationEvent(ApplicationEvent &event) override;

   private:
    RendererSystemD3D11 *m_renderer;
};
} // namespace PPGE
