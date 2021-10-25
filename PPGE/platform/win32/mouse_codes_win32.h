#pragma once
#include "PPGEpch.h"

namespace PPGE
{
enum class MouseCode : uint16_t
{
    Button0 = VK_LBUTTON,
    Button1 = VK_RBUTTON,
    Button2 = VK_MBUTTON,
    Button3 = VK_XBUTTON1,
    Button4 = VK_XBUTTON2,
    Button5 = VK_LBUTTON,
    Button6 = VK_RBUTTON,
    Button7 = VK_MBUTTON,
    ButtonLast = Button7,
    ButtonLeft = Button0,
    ButtonRight = Button1,
    ButtonMiddle = Button2
};

inline std::ostream &operator<<(std::ostream &os, MouseCode mousecode)
{
    os << static_cast<int32_t>(mousecode);
    return os;
}
} // namespace PPGE

#define PPGE_MOUSE_BUTTON_0 ::PPGE::MouseCode::Button0
#define PPGE_MOUSE_BUTTON_1 ::PPGE::MouseCode::Button1
#define PPGE_MOUSE_BUTTON_2 ::PPGE::MouseCode::Button2
#define PPGE_MOUSE_BUTTON_3 ::PPGE::MouseCode::Button3
#define PPGE_MOUSE_BUTTON_4 ::PPGE::MouseCode::Button4
#define PPGE_MOUSE_BUTTON_5 ::PPGE::MouseCode::Button5
#define PPGE_MOUSE_BUTTON_6 ::PPGE::MouseCode::Button6
#define PPGE_MOUSE_BUTTON_7 ::PPGE::MouseCode::Button7
#define PPGE_MOUSE_BUTTON_LAST ::PPGE::MouseCode::ButtonLast
#define PPGE_MOUSE_BUTTON_LEFT ::PPGE::MouseCode::ButtonLeft
#define PPGE_MOUSE_BUTTON_RIGHT ::PPGE::MouseCode::ButtonRight
#define PPGE_MOUSE_BUTTON_MIDDLE ::PPGE::MouseCode::ButtonMiddle
