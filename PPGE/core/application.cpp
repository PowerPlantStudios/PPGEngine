#include "application.h"

#include "core/os_helper.h"

namespace PPGE
{
ApplicationBase *ApplicationBase::s_instance = nullptr;

ApplicationBase::ApplicationBase()
{
    PPGE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    LoggerSystem::Initialize();
    IOSystem::Initialize(GetOSType());
    DisplaySystem::Initialize(GetPreferredWindowAPIType());
    RendererSystem::Initialize(GetPreferredRendererAPIType());
    WidgetSystem::Initialize();
}

ApplicationBase::~ApplicationBase()
{
    WidgetSystem::Destroy();
    RendererSystem::Destroy();
    DisplaySystem::Destroy();
    IOSystem::Destroy();
    LoggerSystem::Destroy();
}

void ApplicationBase::StartUp()
{
    LoggerSystemProps logger_sys_props;
    LoggerSystem::Get().StartUp(logger_sys_props);
    PPGE_INFO("PPGE is starting up.");

    IOSystemProps io_sys_props;
    IOSystem::Get().StartUp(io_sys_props);

    DisplaySystemProps ds_props;
    ds_props.input_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(ApplicationBase::OnInputEvent);
    ds_props.application_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(ApplicationBase::OnApplicationEvent);
    DisplaySystem::Get().StartUp(ds_props);

    RendererSystemProps rs_props;
    RendererSystem::Get().StartUp(rs_props);

    WidgetSystemProps ws_props;
    ws_props.enable_imgui_debug_layer = true;
    WidgetSystem::Get().StartUp(ws_props);
}

void ApplicationBase::ShutDown()
{
    WidgetSystem::Get().ShutDown();
    RendererSystem::Get().ShutDown();
    DisplaySystem::Get().ShutDown();
    IOSystem::Get().ShutDown();

    PPGE_INFO("PPGE is shuting down.");
    LoggerSystem::Get().ShutDown();
}

void ApplicationBase::Run()
{
    while (b_is_running)
    {
        m_game_timer.Tick();
        float delta_time = m_game_timer.GetDeltaTime();

        DisplaySystem::Get().Update();
        WidgetSystem::Get().Update(delta_time);
        RendererSystem::Get().Update();
    }
}

void ApplicationBase::OnInputEvent(InputEvent &input_event)
{
    WidgetSystem::Get().OnInputEvent(input_event);
}

void ApplicationBase::OnApplicationEvent(ApplicationEvent &application_event)
{
    DispatchApplicationEvent<WindowCloseEvent>(application_event,
                                               PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(ApplicationBase::OnWindowClose));
    DispatchApplicationEvent<WindowResizeEvent>(application_event,
                                                PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(ApplicationBase::OnWindowResize));
}

bool ApplicationBase::OnWindowClose(WindowCloseEvent &)
{
    b_is_running = false;

    return true;
}

bool ApplicationBase::OnWindowResize(WindowResizeEvent &)
{
    b_is_paused = (DisplaySystem::Get().IsMinimized()) ? true : false;
    if (b_is_paused)
        m_game_timer.Pause();
    else
        m_game_timer.Resume();

    RendererSystem::Get().OnResize();

    return true;
}

} // namespace PPGE
