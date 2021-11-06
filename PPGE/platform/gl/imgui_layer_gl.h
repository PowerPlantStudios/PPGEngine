#pragma once
#include "PPGEpch.h"

#include "ui/imgui_layer.h"

namespace PPGE
{
class PPGE_API ImGuiLayerGL : public ImGuiLayer
{
  public:
    ImGuiLayerGL() : ImGuiLayer("ImGui Layer (GL)")
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
