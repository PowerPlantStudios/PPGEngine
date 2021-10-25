#pragma once
#include "PPGEpch.h"

namespace PPGE
{
enum class JoystickCode : uint16_t
{
    // From glfw3.h
    Joystick0 = 0,
    Joystick1 = 1,
    Joystick2 = 2,
    Joystick3 = 3,
    Joystick4 = 4,
    Joystick5 = 5,
    Joystick6 = 6,
    Joystick7 = 7,
    Joystick8 = 8,
    Joystick9 = 9,
    Joystick10 = 10,
    Joystick11 = 11,
    Joystick12 = 12,
    Joystick13 = 13,
    Joystick14 = 14,
    Joystick15 = 15,
    JoystickLast = Joystick15,
};

inline std::ostream &operator<<(std::ostream &os, JoystickCode joystick_code)
{
    os << static_cast<int32_t>(joystick_code);
    return os;
}
} // namespace PPGE

#define PPGE_JOYSTICK_BUTTON_0 ::PPGE::JoystickCode::Joystick0
#define PPGE_JOYSTICK_BUTTON_1 ::PPGE::JoystickCode::Joystick1
#define PPGE_JOYSTICK_BUTTON_2 ::PPGE::JoystickCode::Joystick2
#define PPGE_JOYSTICK_BUTTON_3 ::PPGE::JoystickCode::Joystick3
#define PPGE_JOYSTICK_BUTTON_4 ::PPGE::JoystickCode::Joystick4
#define PPGE_JOYSTICK_BUTTON_5 ::PPGE::JoystickCode::Joystick5
#define PPGE_JOYSTICK_BUTTON_6 ::PPGE::JoystickCode::Joystick6
#define PPGE_JOYSTICK_BUTTON_7 ::PPGE::JoystickCode::Joystick7
#define PPGE_JOYSTICK_BUTTON_8 ::PPGE::JoystickCode::Joystick8
#define PPGE_JOYSTICK_BUTTON_9 ::PPGE::JoystickCode::Joystick9
#define PPGE_JOYSTICK_BUTTON_10 ::PPGE::JoystickCode::Joystick10
#define PPGE_JOYSTICK_BUTTON_11 ::PPGE::JoystickCode::Joystick11
#define PPGE_JOYSTICK_BUTTON_12 ::PPGE::JoystickCode::Joystick12
#define PPGE_JOYSTICK_BUTTON_13 ::PPGE::JoystickCode::Joystick13
#define PPGE_JOYSTICK_BUTTON_14 ::PPGE::JoystickCode::Joystick14
#define PPGE_JOYSTICK_BUTTON_15 ::PPGE::JoystickCode::Joystick15
#define PPGE_JOYSTICK_BUTTON_LAST ::PPGE::JoystickCode::JoystickLast
