#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"
#include "core/smart_ptr.h"
#include "ui/imgui_layer.h"
#include "ui/ui_layer.h"
#include "systems/display_system.h"
#include "systems/ui_system.h"

namespace PPGE
{
class PPGE_API Application
{
  public:
    Application();
    virtual ~Application();

    void OnInputEvent(InputEvent &input_event);
    void OnApplicationEvent(ApplicationEvent &application_event);
    void Run();

    void RegisterSubsystemToFrontQueue(UILayer *subsystem);
    void RegisterSubsystemToBackQueue(UILayer *subsystem);

    static Application &Get()
    {
        return *s_instance;
    }

  private:
    bool OnWindowClose(WindowCloseEvent &win_close_event);
    bool OnWindowResize(WindowResizeEvent &win_resize_event);

    ImGuiLayer *m_imgui_subsystem;
    bool m_is_running = true;
    bool m_is_paused = false;

    static Application *s_instance;
};

// Declaration to be implemented on client code
Application *CreateApplication();
} // namespace PPGE
