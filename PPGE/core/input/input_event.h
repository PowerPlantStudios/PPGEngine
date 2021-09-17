#pragma once
#include "PPGEpch.h"

#include "Core/defines.h"
#include "Core/input/input.h"

namespace PPGE
{
	enum class InputEventType
	{
		None = 0,
		KeyPressed,
		KeyReleased,
		KeyTyped,
		MouseMoved,
		MouseScrolled,
		MousePressed,
		MouseReleased,
	};

	enum class InputEventCategory
	{
		None = 0,
		Mouse,
		Keyboard,
		Joystick,
	};

	enum class InputEventCategoryBit
	{
		None     = (1 << static_cast<int>(InputEventCategory::None)),
		Mouse    = (1 << static_cast<int>(InputEventCategory::Mouse)),
		Keyboard = (1 << static_cast<int>(InputEventCategory::Keyboard)),
		Joystick = (1 << static_cast<int>(InputEventCategory::Joystick)),
	};

#define PPGE_INPUT_EVENT_CATEGORY(category)  virtual int GetInputEventCategory() const override { return category; }
#define PPGE_INPUT_EVENT_CLASS(type)         static  InputEventType RetreiveInputEventTypeEnum() { return InputEventType::type; }                \
								             virtual InputEventType GetInputEventType() const override { return RetreiveInputEventTypeEnum(); }  \
								             virtual const char* Name() const override { return #type; }                                         \
	
	class PPGE_API InputEvent
	{
	public:
		virtual ~InputEvent() = default;
		
		static InputEventType RetreiveInputEventTypeEnum() { return InputEventType::None; }

		virtual InputEventType GetInputEventType() const = 0;
		virtual int GetInputEventCategory() const = 0;
		virtual const char* Name() const = 0;
		virtual std::string ToString() const { return Name(); }

		bool IsInCategory(InputEventCategoryBit category) { return GetInputEventCategory() & static_cast<int>(category); }
		
		void SetHandled(bool b_value) { b_handled |= b_value; }
		bool Handled() { return b_handled; }

	private:
		bool b_handled = false;
	};

	class PPGE_API KeyPressedEvent : public InputEvent
	{
	public:
		KeyPressedEvent(KeyCode keycode, int repeat) : m_keycode(keycode), m_repeat(repeat) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Keyboard))
		PPGE_INPUT_EVENT_CLASS(KeyPressed)

		KeyCode Keycode() const { return m_keycode; }
		int Repeat() const { return m_repeat; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Pressed Event -> Keycode : " << m_keycode << " , Repeat : " << m_repeat;
			return ss.str();
		}

	private:
		KeyCode m_keycode;
		int m_repeat;
	};

	class PPGE_API KeyReleasedEvent : public InputEvent
	{
	public:
		KeyReleasedEvent(KeyCode keycode) : m_keycode(keycode) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Keyboard))
		PPGE_INPUT_EVENT_CLASS(KeyReleased)

		KeyCode Keycode() const { return m_keycode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Released Event -> Keycode : " << m_keycode;
			return ss.str();
		}

	private:
		KeyCode m_keycode;
	};

	class PPGE_API KeyTypedEvent : public InputEvent
	{
	public:
		KeyTypedEvent(KeyCode keycode) : m_keycode(keycode) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Keyboard))
		PPGE_INPUT_EVENT_CLASS(KeyTyped)

		KeyCode Keycode() const { return m_keycode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Key Typed Event -> Keycode : " << m_keycode;
			return ss.str();
		}

	private:
		KeyCode m_keycode;
	};

	class PPGE_API MouseMovedEvent : public InputEvent
	{
	public:
		MouseMovedEvent(float x, float y) : m_posX(x), m_posY(y) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Mouse))
		PPGE_INPUT_EVENT_CLASS(MouseMoved)

		float PosX() const { return m_posX; }
		float PosY() const { return m_posY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Move Event -> PosX : " << m_posX << " , PosY : " << m_posY;
			return ss.str();
		}

	private:
		float m_posX, m_posY;
	};

	class PPGE_API MouseScrolledEvent : public InputEvent
	{
	public:
		MouseScrolledEvent(float deltaX, float deltaY) : m_deltaX(deltaX), m_deltaY(deltaY) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Mouse))
		PPGE_INPUT_EVENT_CLASS(MouseScrolled)

		float deltaX() const { return m_deltaX; }
		float deltaY() const { return m_deltaY; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Scroll Event -> delta X : " << m_deltaX << " , delta Y: " << m_deltaY;
			return ss.str();
		}

	private:
		float m_deltaX, m_deltaY;
	};

	class PPGE_API MouseButtonPressedEvent : public InputEvent
	{
	public:
		MouseButtonPressedEvent(MouseCode mousecode) : m_mousecode(mousecode) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Mouse))
		PPGE_INPUT_EVENT_CLASS(MousePressed)

		MouseCode Mousecode() const { return m_mousecode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Pressed Event -> Mousecode : " << m_mousecode;
			return ss.str();
		}

	private:
		MouseCode m_mousecode;
	};

	class PPGE_API MouseButtonReleasedEvent : public InputEvent
	{
	public:
		MouseButtonReleasedEvent(MouseCode mousecode) : m_mousecode(mousecode) {}

		PPGE_INPUT_EVENT_CATEGORY(static_cast<int>(InputEventCategoryBit::Mouse))
		PPGE_INPUT_EVENT_CLASS(MouseReleased)

		MouseCode Mousecode() const { return m_mousecode; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "Mouse Released Event -> Mousecode : " << m_mousecode;
			return ss.str();
		}

	private:
		MouseCode m_mousecode;
	};

	inline std::ostream& operator<<(std::ostream& os, const InputEvent& e)
	{
		os << e.ToString();
		return os;
	}

    template<typename T, typename F>
    bool DispatchInputEvent(const InputEvent& e, const F& func)
    {
        if (e.GetInputEventType() == T::RetreiveInputEventTypeEnum())
        {
            e.SetHandled(func(static_cast<T&>(e)));
            return true;
        }
        return false;
    }
}