#include "PPGEpch.h"

#include "application.h"

#include <glad/glad.h>

namespace PPGE
{
	Application* Application::s_instance = nullptr;

	Application::Application() : b_isRuning(true), b_isPaused(false)
	{
		PPGE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		DisplaySystem::DisplaySystemProps props; //Title, Width and Height properties can be set before creating display
		m_appDisplay = DisplaySystem::Create(props);
		m_appDisplay->SetInputEventCallbackFunctionPtr(PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnInputEvent));
		m_appDisplay->SetApplicationEventCallbackFunctionPtr(PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnApplicationEvent));
	}

	Application::~Application()
	{
		PPGE_INFO("PPGE is destroyed.");
	}

	void Application::OnInputEvent(InputEvent& inputEvent)
	{
		// PPGE_TRACE(inputEvent.ToString());
		for (auto it = m_subsystemMgr.rbegin(); it != m_subsystemMgr.rend(); ++it)
		{
			if (inputEvent.Handled())
				break;
			(*it)->OnInputEvent(inputEvent);
		}
	}

	void Application::OnApplicationEvent(ApplicationEvent& appEvent)
	{
		// PPGE_TRACE(appEvent.ToString());
		DispatchApplicationEvent<WindowCloseEvent>(appEvent, PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowClose));
		DispatchApplicationEvent<WindowResizeEvent>(appEvent, PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowResize));
	}

	void Application::Run()
	{
		while (b_isRuning)
		{
			{
				for (Subsystem* subsys : m_subsystemMgr)
					subsys->OnUpdate(0.0f);
			}

			{
			
			}

			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_appDisplay->OnUpdate();
		}
	}

	void Application::RegisterSubsystemToFrontQueue(Subsystem* subsystem)
	{
		m_subsystemMgr.PushFrontQueue(subsystem);
		subsystem->OnAttach();
	}

	void Application::RegisterSubsystemToBackQueue(Subsystem* subsystem)
	{
		m_subsystemMgr.PushBackQueue(subsystem);
		subsystem->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& winCloseEvent)
	{
		b_isRuning = false;
		
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& winResizeEvent)
	{
		b_isPaused = (m_appDisplay->IsMinimized()) ? true : false;

		return true;
	}

}
