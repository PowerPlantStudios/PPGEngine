#include "PPGEpch.h"

#include "application.h"

#include "GLFW/glfw3.h"

namespace PPGE
{
	Application::Application() : b_isRuning(true), b_isPaused(false)
	{
		DisplaySystem::DisplaySystemProps props; //Title, Widht and Height properties can be set before creating display
		m_appDisplay = DisplaySystem::Create(props);
		m_appDisplay->SetInputEventCallbackFunctionPtr(PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnInputEvent));
		m_appDisplay->SetApplicationEventCallbackFunctionPtr(PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnApplicationEvent));
	}

	Application::~Application()
	{
		PPGE_INFO("PPGE is destroyed.");
	}

	void Application::OnInputEvent(const InputEvent& inputEvent)
	{
		PPGE_TRACE(inputEvent);
	}

	void Application::OnApplicationEvent(const ApplicationEvent& appEvent)
	{
		PPGE_TRACE(appEvent);
		DispatchApplicationEvent<WindowCloseEvent>(appEvent, PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowClose));
		DispatchApplicationEvent<WindowResizeEvent>(appEvent, PPGE_BIND_CLASS_METHOD_ARG_COUNT_1(Application::OnWindowResize));
	}

	void Application::Run()
	{
		while (b_isRuning)
		{
			glClearColor(1, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_appDisplay->OnUpdate();
		}
	}

	bool Application::OnWindowClose(const WindowCloseEvent& winCloseEvent)
	{
		b_isRuning = false;
		
		return true;
	}

	bool Application::OnWindowResize(const WindowResizeEvent& winResizeEvent)
	{
		b_isPaused = (m_appDisplay->IsMinimized()) ? true : false;

		return true;
	}

}
