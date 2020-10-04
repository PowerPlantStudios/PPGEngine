#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/smart_ptr.h"
#include "systems/display_system.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"

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
