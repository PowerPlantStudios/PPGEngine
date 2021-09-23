#include "PPGEpch.h"

#include "application.h"

#include <glad/glad.h>

namespace PPGE
{
Application *Application::s_instance = nullptr;

Application::Application()
{
    PPGE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    LoggerSystemProps logger_sys_props;
    LoggerSystem::GetLoggerSystem().StartUp(logger_sys_props);
    PPGE_INFO("PPGE is initialized.");

    DisplaySystemProps ds_props;
    ds_props.input_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnInputEvent);
    ds_props.application_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnApplicationEvent);
    DisplaySystem::GetDisplaySystem().StartUp(ds_props);

    UISystemProps ls_props;
    UISystem::GetLayerSystem().StartUp(ls_props);

    m_imgui_subsystem = ImGuiLayer::CreateImGuiLayer();
    RegisterSubsystemToFrontQueue(m_imgui_subsystem);
}

Application::~Application()
{
    PPGE_INFO("PPGE is destroyed.");
    UISystem::GetLayerSystem().ShutDown();
    DisplaySystem::GetDisplaySystem().ShutDown();
    LoggerSystem::GetLoggerSystem().ShutDown();
}

void Application::OnInputEvent(InputEvent &input_event)
{
    // PPGE_TRACE(inputEvent.ToString());
    for (auto it = UISystem::GetLayerSystem().rbegin(); it != UISystem::GetLayerSystem().rend(); ++it)
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

void Application::Run()
{
    while (m_is_running)
    {
        {
            for (UILayer *subsys : UISystem::GetLayerSystem())
                subsys->OnUpdate(0.0f);
        }

        m_imgui_subsystem->OnImGuiBegin();
        {
        }
        m_imgui_subsystem->OnRender();

        DisplaySystem::GetDisplaySystem().Update();
    }
}

void Application::RegisterSubsystemToFrontQueue(UILayer *subsystem)
{
    UISystem::GetLayerSystem().PushFrontQueue(subsystem);
    subsystem->OnAttach();
}

void Application::RegisterSubsystemToBackQueue(UILayer *subsystem)
{
    UISystem::GetLayerSystem().PushBackQueue(subsystem);
    subsystem->OnAttach();
}

bool Application::OnWindowClose(WindowCloseEvent &)
{
    m_is_running = false;

    return true;
}

bool Application::OnWindowResize(WindowResizeEvent &)
{
    m_is_paused = (DisplaySystem::GetDisplaySystem().IsMinimized()) ? true : false;

    return true;
}

} // namespace PPGE
