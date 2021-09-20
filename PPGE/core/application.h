#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/imgui/imgui_subsytem.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"
#include "core/smart_ptr.h"
#include "core/subsystem_manager.h"
#include "systems/display_system.h"
#include "systems/subsystem.h"

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

    void RegisterSubsystemToFrontQueue(Subsystem *subsystem);
    void RegisterSubsystemToBackQueue(Subsystem *subsystem);

    static Application &Get()
    {
        return *s_instance;
    }

  private:
    bool OnWindowClose(WindowCloseEvent &win_close_event);
    bool OnWindowResize(WindowResizeEvent &win_resize_event);

    ImGuiSubsystem *m_imgui_subsystem;
    SubsystemManager m_subsystem_manager;
    bool m_is_running = true;
    bool m_is_paused = false;

    static Application *s_instance;
};

// Declaration to be implemented on client code
Application *CreateApplication();
} // namespace PPGE
