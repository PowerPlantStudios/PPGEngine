#include "application.h"

namespace PPGE
{
Application *Application::s_instance = nullptr;

Application::Application()
{
    PPGE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    LoggerSystem::Initialize();
    DisplaySystem::Initialize(WindowAPI::Win32);
    RendererSystem::Initialize(RendererAPI::DX11);
    UISystem::Initialize();
}

Application::~Application()
{
    LoggerSystem::Destroy();
    DisplaySystem::Destroy();
    RendererSystem::Destroy();
    UISystem::Destroy();
}

void Application::StartUp()
{
    LoggerSystemProps logger_sys_props;
    LoggerSystem::Get().StartUp(logger_sys_props);
    PPGE_INFO("PPGE is starting up.");

    DisplaySystemProps ds_props;
    ds_props.input_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnInputEvent);
    ds_props.application_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnApplicationEvent);
    DisplaySystem::Get().StartUp(ds_props);

    RendererSystemProps rs_props;
    RendererSystem::Get().StartUp(rs_props);

    UISystemProps ls_props;
    UISystem::Get().StartUp(ls_props);
}

void Application::ShutDown()
{
    PPGE_INFO("PPGE is shuting down.");

    UISystem::Get().ShutDown();
    RendererSystem::Get().ShutDown();
    DisplaySystem::Get().ShutDown();
    LoggerSystem::Get().ShutDown();
}

void Application::Run()
{
    while (b_is_running)
    {
        m_game_timer.Tick();
        float m_delta_time = m_game_timer.GetDeltaTime();

        DisplaySystem::Get().Update();

        RendererSystem::Get().ClearColor(0.15, 0.15, 0.25);
        RendererSystem::Get().ClearDepthStencilBuffer(1.0, 0);
        {
            for (auto &&subsys : UISystem::Get())
                subsys->OnUpdate(m_delta_time);
        }
        RendererSystem::Get().Update();
    }
}

void Application::OnInputEvent(InputEvent &input_event)
{
    // PPGE_TRACE(input_event.ToString());
    for (auto it = UISystem::Get().rbegin(); it != UISystem::Get().rend(); ++it)
    {
        if (input_event.Handled())
            break;
        (*it)->OnInputEvent(input_event);
    }
}

void Application::OnApplicationEvent(ApplicationEvent &application_event)
{
    // PPGE_TRACE(appEvent.ToString());
    DispatchApplicationEvent<WindowCloseEvent>(application_event,
                                               PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowClose));
    DispatchApplicationEvent<WindowResizeEvent>(application_event,
                                                PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowResize));
}

void Application::PushLayerFront(std::unique_ptr<UILayer> layer)
{
    UISystem::Get().PushLayerFront(std::move(layer));
}

void Application::PushLayerBack(std::unique_ptr<UILayer> layer)
{
    UISystem::Get().PushLayerBack(std::move(layer));
}

bool Application::OnWindowClose(WindowCloseEvent &)
{
    b_is_running = false;

    return true;
}

bool Application::OnWindowResize(WindowResizeEvent &)
{
    b_is_paused = (DisplaySystem::Get().IsMinimized()) ? true : false;
    if (b_is_paused)
        m_game_timer.Pause();
    else
        m_game_timer.Resume();

    return true;
}

} // namespace PPGE
