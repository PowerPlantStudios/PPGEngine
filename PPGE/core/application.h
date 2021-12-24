#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/game_timer.h"
#include "core/widget.h"
#include "event/application_event.h"
#include "event/input_event.h"
#include "system/display_system.h"
#include "system/renderer_system.h"
#include "system/widget_system.h"

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

    template <class WidgetType, class... Args, std::enable_if_t<std::is_base_of_v<Widget, WidgetType>, int> = 0>
    void CreateWidget(Args &&... args)
    {
        WidgetSystem::Get().CreateWidget<WidgetType>(std::forward<Args>(args)...);
    }

  private:
    bool OnWindowClose(WindowCloseEvent &win_close_event);
    bool OnWindowResize(WindowResizeEvent &win_resize_event);

    bool b_is_running = true;
    bool b_is_paused = false;

    GameTimer m_game_timer;

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
