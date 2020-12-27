#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/smart_ptr.h"
#include "systems/display_system.h"
#include "core/input/application_event.h"
#include "core/input/input_event.h"
#include "core/subsystem_manager.h"
#include "core/subsystem.h"

namespace PPGE
{
	class PPGE_API Application
	{
	public:
		Application();
		virtual ~Application();
		
		void OnInputEvent(InputEvent& inputEvent);
		void OnApplicationEvent(ApplicationEvent& appEvent);
		void Run();

		DisplaySystem& GetDisplay() { return *m_appDisplay; }

		void RegisterSubsystemToFrontQueue(Subsystem* subsystem);
		void RegisterSubsystemToBackQueue(Subsystem* subsystem);

		static Application& Get() { return *s_instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& winCloseEvent);
		bool OnWindowResize(WindowResizeEvent& winResizeEvent);

	private:
		Unique<DisplaySystem> m_appDisplay;
		SubsystemManager m_subsystemMgr;
		bool b_isRuning;
		bool b_isPaused;

	private:
		static Application* s_instance;
	};

	// Declaration to be implemented on client code
	Application* CreateApplication();
}
