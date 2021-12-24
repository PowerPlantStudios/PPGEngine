#pragma once
#include "PPGEpch.h"

#include "ui/imgui_widget.h"

namespace PPGE
{
class PPGE_API ImGuiWidgetGL : public ImGuiWidget
{
  public:
    ImGuiWidgetGL() : ImGuiWidget("ImGui Layer (GL)")
    {
    }

    void OnAttach() override;

    void OnDetach() override;

    void ImGuiBegin() override;

    void ImGuiEnd() override;

    void OnInputEvent(InputEvent &event) override;

    void OnApplicationEvent(ApplicationEvent &event) override;
};
} // namespace PPGE
