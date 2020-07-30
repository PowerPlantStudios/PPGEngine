#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

namespace PPGE
{
	enum class ApplicationEventType
	{
		None = 0,
		WindowResized,
		WindowClosed,
		WindowMoved,
		AppRendered,
		AppUpdated,
	};

	enum class ApplicationEventCategory
	{
		None = 0,
		Window,
		Application
	};

	enum class ApplicationEventCategoryBit
	{
		None        = (1 << static_cast<int>(ApplicationEventCategory::None)),
		Window      = (1 << static_cast<int>(ApplicationEventCategory::Window)),
		Application = (1 << static_cast<int>(ApplicationEventCategory::Application)),
	};

#define PPGE_APPLICATION_EVENT_CATEGORY(category)  virtual int GetApplicationEventCategory() const override { return category; }
#define PPGE_APPLICATION_EVENT_CLASS(type)         static  ApplicationEventType RetreiveApplicationEventTypeEnum() { return ApplicationEventType::type; }                \
								                   virtual ApplicationEventType GetApplicationEventType() const override { return RetreiveApplicationEventTypeEnum(); }  \
								                   virtual const char* Name() const override { return #type; }                                                           \
	
	class PPGE_API ApplicationEvent;

	template<typename T, typename F>
	bool DispatchInputEvent(const ApplicationEvent& e, const F& func)
	{
		if (e.GetInputEventType() == T::RetreiveInputEventTypeEnum())
		{
			e.SetHandled(func(static_cast<T&>(e)));
			return true;
		}
		return false;
	}

	class PPGE_API ApplicationEvent
	{
	public:
		virtual ~ApplicationEvent() = default;

		static ApplicationEventType RetreiveApplicationEventTypeEnum() { return ApplicationEventType::None; }

		virtual ApplicationEventType GetApplicationEventType() const = 0;
		virtual int GetApplicationEventCategory() const = 0;
		virtual const char* Name() const = 0;
		virtual std::string ToString() const { return Name(); }

		bool IsInCategory(ApplicationEventCategoryBit category) { return GetApplicationEventCategory() & static_cast<int>(category); }

		void SetHandled(bool b_value) { b_handled = b_value; }
		bool Handled() { return b_handled; }

	private:
		bool b_handled = false;
	};

	class PPGE_API WindowResizeEvent : public ApplicationEvent
	{
	public:
		WindowResizeEvent(uint32_t width, uint32_t height) : m_width(width), m_height(height) {}

		PPGE_APPLICATION_EVENT_CATEGORY(static_cast<int>(ApplicationEventCategoryBit::Window))
		PPGE_APPLICATION_EVENT_CLASS(WindowResized)

		uint32_t Width() const { return m_width; }
		uint32_t Height() const { return m_height; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Resize Event -> Width : " << m_width << ", Height" << m_height;
			return ss.str();
		}

	private:
		uint32_t m_width, m_height;
	};

	class PPGE_API WindowMoveEvent : public ApplicationEvent
	{
	public:
		WindowMoveEvent(uint32_t xPos, uint32_t yPos) : m_xPos(xPos), m_yPos(yPos) {}

		PPGE_APPLICATION_EVENT_CATEGORY(static_cast<int>(ApplicationEventCategoryBit::Window))
		PPGE_APPLICATION_EVENT_CLASS(WindowMoved)

		uint32_t XPos() const { return m_xPos; }
		uint32_t YPos() const { return m_yPos; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Move Event -> X Position : " << m_xPos << ", Y Position" << m_yPos;
			return ss.str();
		}

	private:
		uint32_t m_xPos, m_yPos;
	};

	class PPGE_API WindowCloseEvent : public ApplicationEvent
	{
	public:
		WindowCloseEvent() = default;

		PPGE_APPLICATION_EVENT_CATEGORY(static_cast<int>(ApplicationEventCategoryBit::Window))
		PPGE_APPLICATION_EVENT_CLASS(WindowClosed)

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Window Close Event";
			return ss.str();
		}
	};

	inline std::ostream& operator<<(std::ostream& os, const ApplicationEvent& e)
	{
		return os << e.ToString();
	}
}