#pragma once
#include "PPGEpch.h"

#include "Core/defines.h"
#include "Core/smart_ptr.h"
#include "systems/display_system.h"
#include "Core/input/application_event.h"
#include "Core/input/input_event.h"

namespace PPGE
{
	class PPGE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void OnInputEvent(const InputEvent& inputEvent);
		void OnApplicationEvent(const ApplicationEvent& appEvent);
		void Run();
		
	private:
		bool OnWindowClose(const WindowCloseEvent& winCloseEvent);
		bool OnWindowResize(const WindowResizeEvent& winResizeEvent);

	private:
		Unique<DisplaySystem> m_appDisplay;
		bool b_isRuning;
		bool b_isPaused;
	};

	// Decleration to be implemented on client code
	Application* CreateApplication();
}
