#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input/input_event.h"
#include "systems/subsystem.h"

namespace PPGE
{
class PPGE_API ImGuiSubsystem : public Subsystem
{
  public:
    ImGuiSubsystem();
    ~ImGuiSubsystem();

    void OnAttach() override;
    void OnDetach() override;
    void OnInputEvent(InputEvent &input_event) override;

    void InitFrame();
    void RenderFrame();

  private:
    bool show_demo_window = true;
};
} // namespace PPGE