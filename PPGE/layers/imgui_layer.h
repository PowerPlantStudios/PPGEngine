#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input/input_event.h"
#include "layers/layer.h"

namespace PPGE
{
class PPGE_API ImGuiLayer : public Layer
{
  public:
    ImGuiLayer();
    ~ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnInputEvent(InputEvent &input_event) override;

    void InitFrame();
    void RenderFrame();

  private:
    bool show_demo_window = true;
};
} // namespace PPGE