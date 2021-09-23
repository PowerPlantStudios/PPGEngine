#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input/input_event.h"
#include "ui/ui_layer.h"

namespace PPGE
{
class PPGE_API ImGuiLayer : public UILayer
{
  public:
    // Static function to create ImGui Layer dynamically for the current platform
    static ImGuiLayer *CreateImGuiLayer();

    ImGuiLayer(const std::string& name) : UILayer(name)
    {
    }

    virtual void OnImGuiBegin()
    {
    }
};
} // namespace PPGE
