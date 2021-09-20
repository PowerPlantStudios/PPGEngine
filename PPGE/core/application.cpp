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

    DisplaySystemProps props;
    props.input_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnInputEvent);
    props.application_event_callback = PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnApplicationEvent);
    DisplaySystem::GetDisplaySystem().StartUp(props);

    m_imgui_subsystem = new ImGuiSubsystem();
    RegisterSubsystemToFrontQueue(m_imgui_subsystem);
}

Application::~Application()
{
    DisplaySystem::GetDisplaySystem().ShutDown();
    PPGE_INFO("PPGE is destroyed.");
}

void Application::OnInputEvent(InputEvent &input_event)
{
    // PPGE_TRACE(inputEvent.ToString());
    for (auto it = m_subsystem_manager.rbegin(); it != m_subsystem_manager.rend(); ++it)
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
            for (Subsystem *subsys : m_subsystem_manager)
                subsys->OnUpdate(0.0f);
        }

        m_imgui_subsystem->InitFrame();
        {
        }
        m_imgui_subsystem->RenderFrame();

        DisplaySystem::GetDisplaySystem().Update();
    }
}

void Application::RegisterSubsystemToFrontQueue(Subsystem *subsystem)
{
    m_subsystem_manager.PushFrontQueue(subsystem);
    subsystem->OnAttach();
}

void Application::RegisterSubsystemToBackQueue(Subsystem *subsystem)
{
    m_subsystem_manager.PushBackQueue(subsystem);
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
