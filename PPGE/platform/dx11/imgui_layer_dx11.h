#pragma once
#include "PPGEpch.h"

#include "ui/imgui_layer.h"

namespace PPGE
{
class ImGuiLayerDX11 : public ImGuiLayer
{
  public:
    ImGuiLayerDX11() : ImGuiLayer("ImGui Layer")
    {
    }

    void OnAttach() override;

    void OnDetach() override;

    void OnImGuiBegin() override;

    void OnRender() override;

    void OnInputEvent(InputEvent &event) override;

    void OnApplicationEvent(ApplicationEvent &event) override;
};
} // namespace PPGE
