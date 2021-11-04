#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "system/display_system.h"
#include "system/renderer_system.h"
#include "system/ui_system.h"
#include "ui/imgui_layer.h"
#include "ui/ui_layer.h"

namespace PPGE
{
class PPGE_API Application
{
  public:
    Application();
    virtual ~Application();

    virtual void StartUp();
    virtual void ShutDown();
    virtual void Run();

    void OnInputEvent(InputEvent &input_event);
    void OnApplicationEvent(ApplicationEvent &application_event);

    void PushLayerFront(std::unique_ptr<UILayer> layer);
    void PushLayerBack(std::unique_ptr<UILayer> layer);

  private:
    bool OnWindowClose(WindowCloseEvent &win_close_event);
    bool OnWindowResize(WindowResizeEvent &win_resize_event);

    std::weak_ptr<ImGuiLayer> m_imgui_layer;
    bool b_is_running = true;
    bool b_is_paused = false;

  public:
    static Application &Get()
    {
        return *s_instance;
    }

  private:
    static Application *s_instance;
};

// Declaration to be implemented on client code
Application *CreateApplication();
} // namespace PPGE
