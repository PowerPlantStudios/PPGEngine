#pragma once
#include "PPGEpch.h"

#include "Core/defines.h"
#include "Core/logger.h"
#include "Core/smart_ptr.h"
#include "Core/input/input_event.h"
#include "Core/input/application_event.h"

namespace PPGE
{
	class PPGE_API DisplaySystem
	{
	public:

		//typedef void (*InputEventCallback)(const InputEvent&);
		//typedef void (*ApplicationEventCallback)(const ApplicationEvent&);

		typedef std::function<void(const InputEvent&)> InputEventCallback;
		typedef std::function<void(const ApplicationEvent&)> ApplicationEventCallback;
		
		enum class WindowFlags
		{
			ALWAYS_ON_TOP,
			DECORATED,
			RESIZABLE,
			FOCUSED,
		};

		enum class WindowModes {
			WINDOWED,
			MINIMIZED,
			MAXIMIZED,
			FULLSCREEN
		};

		struct DisplaySystemProps
		{
			std::string title = "Power Plant Game Engine";
			uint32_t height   = 720;
			uint32_t width    = 1280;
		};

		static Unique<DisplaySystem> Create(const DisplaySystemProps&);

	public:
		virtual ~DisplaySystem() {}

		virtual void OnUpdate() = 0;

		virtual void* GetNativeDisplayPtr() const = 0;

		virtual bool GetWindowFlag(WindowFlags flag) const = 0;
		virtual void SetWindowFlag(WindowFlags flag, bool b_value) = 0;

		virtual void SetWindowMode(WindowModes mode) = 0;

		virtual bool IsVsyncEnabled() const = 0;
		virtual void SetVsync(bool b_value) = 0;
		virtual bool IsMaximized() const = 0;
		virtual bool IsMinimized() const = 0;

		virtual InputEventCallback InputEventCallbackFunctionPtr() const = 0;
		virtual void SetInputEventCallbackFunctionPtr(InputEventCallback callback) = 0;
		virtual ApplicationEventCallback ApplicationEventCallbackFunctionPtr() const = 0;
		virtual void SetApplicationEventCallbackFunctionPtr(ApplicationEventCallback callback) = 0;
		virtual std::string Title() const = 0;
		virtual void Title(const std::string& title) = 0;
		virtual uint32_t Height() const = 0;
		virtual void SetHeight(uint32_t height) = 0;
		virtual uint32_t Width() const = 0;
		virtual void SetWidth(uint32_t wdith) = 0;

	private:

	};
}
