#pragma once
#include "PPGEpch.h"

namespace PPGE
{
	enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0      = 0,
		Button1      = 1,
		Button2      = 2,
		Button3      = 3,
		Button4      = 4,
		Button5      = 5,
		Button6      = 6,
		Button7      = 7,
		ButtonLast   = Button7,
		ButtonLeft   = Button0,
		ButtonRight  = Button1,
		ButtonMiddle = Button2
	};

	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define PPGE_MOUSE_BUTTON_0      ::PPGE::MouseCode::Button0
#define PPGE_MOUSE_BUTTON_1      ::PPGE::MouseCode::Button1
#define PPGE_MOUSE_BUTTON_2      ::PPGE::MouseCode::Button2
#define PPGE_MOUSE_BUTTON_3      ::PPGE::MouseCode::Button3
#define PPGE_MOUSE_BUTTON_4      ::PPGE::MouseCode::Button4
#define PPGE_MOUSE_BUTTON_5      ::PPGE::MouseCode::Button5
#define PPGE_MOUSE_BUTTON_6      ::PPGE::MouseCode::Button6
#define PPGE_MOUSE_BUTTON_7      ::PPGE::MouseCode::Button7
#define PPGE_MOUSE_BUTTON_LAST   ::PPGE::MouseCode::ButtonLast
#define PPGE_MOUSE_BUTTON_LEFT   ::PPGE::MouseCode::ButtonLeft
#define PPGE_MOUSE_BUTTON_RIGHT  ::PPGE::MouseCode::ButtonRight
#define PPGE_MOUSE_BUTTON_MIDDLE ::PPGE::MouseCode::ButtonMiddle
