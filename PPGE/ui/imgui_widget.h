#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/widget.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGE_API ImGuiWidget : public Widget
{
  public:
    // Static function to create ImGui Layer dynamically for the current platform
    static std::unique_ptr<ImGuiWidget> CreateImGuiWidget();
    
    ImGuiWidget(const std::string &name) : Widget(name)
    {
    }

    virtual void ImGuiBegin() = 0;
    virtual void ImGuiEnd() = 0;
};
} // namespace PPGE