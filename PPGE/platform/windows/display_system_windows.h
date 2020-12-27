#pragma once
#include <GLFW/glfw3.h>

#include "Core/defines.h"
#include "Core/logger.h"
#include "systems/display_system.h"

namespace PPGE
{
	class PPGE_API DisplaySystemWindows : public DisplaySystem
	{
	public:

		DisplaySystemWindows(const DisplaySystemProps& props);

		virtual ~DisplaySystemWindows();

		virtual void OnUpdate() override;

		virtual void* GetNativeDisplayPtr() const override;

		virtual bool GetWindowFlag(WindowFlags flag) const override;
		virtual void SetWindowFlag(WindowFlags flag, bool b_value) override;

		virtual void SetWindowMode(WindowModes mode) override;

		virtual bool IsVsyncEnabled() const override;
		virtual void SetVsync(bool b_value) override;
		virtual bool IsMaximized() const override;
		virtual bool IsMinimized() const override;

		virtual InputEventCallback InputEventCallbackFunctionPtr() const override;
		virtual void SetInputEventCallbackFunctionPtr(InputEventCallback callback) override;
		virtual ApplicationEventCallback ApplicationEventCallbackFunctionPtr() const override;
		virtual void SetApplicationEventCallbackFunctionPtr(ApplicationEventCallback callback) override;
		virtual std::string Title() const override;
		virtual void Title(const std::string& title) override;
		virtual uint32_t Height() const override;
		virtual void SetHeight(uint32_t height) override;
		virtual uint32_t Width() const override;
		virtual void SetWidth(uint32_t wdith) override;

	private:

		void Init(const DisplaySystemProps& props);
		void Shutdown();

	private:

		struct DisplaySystemWindowsProps
		{

			std::string title;
			uint32_t height, width;
			uint32_t posX, posY;
			bool vSync, fullscreen, minimized, maximized;

			InputEventCallback InputEventCallback;
			ApplicationEventCallback ApplicationEventCallback;
		};

		GLFWwindow* m_window;
		DisplaySystemWindowsProps m_props;
	};
}
