#pragma once
#include "PPGEpch.h"

namespace PPGE
{
enum class GamepadButtonCode : uint16_t
{
    // From glfw3.h
    ButtonA = 0,
    ButtonB = 1,
    ButtonX = 2,
    ButtonY = 3,
    ButtonLeftBumper = 4,
    ButtonRightBumper = 5,
    ButtonBack = 6,
    ButtonStart = 7,
    ButtonGuide = 8,
    ButtonLeftThumb = 9,
    ButtonRightThumb = 10,
    ButtonDpadUp = 11,
    ButtonDpadRight = 12,
    ButtonDpadDown = 13,
    ButtonDpadLeft = 14,
    ButtonLast = ButtonDpadLeft,
    ButtonCross = ButtonA,
    ButtonCircle = ButtonB,
    ButtonSquare = ButtonX,
    ButtonTriangle = ButtonY,
};

enum class GamepadAxisCode : uint16_t
{
    // From glfw3.h
    AxisLeftX = 0,
    AxisLeftY = 1,
    AxisRightX = 2,
    AxisRightY = 3,
    AxisLeftTrigger = 4,
    AxisRightTrigger = 5,
    AxisLast = AxisRightTrigger,
};

inline std::ostream &operator<<(std::ostream &os, GamepadButtonCode gamepad_button_code)
{
    os << static_cast<int32_t>(gamepad_button_code);
    return os;
}

inline std::ostream &operator<<(std::ostream &os, GamepadAxisCode gamepad_axis_code)
{
    os << static_cast<int32_t>(gamepad_axis_code);
    return os;
}
} // namespace PPGE

#define PPGE_GAMEPAD_BUTTON_A ::PPGE::GamepadButtonCode::ButtonA
#define PPGE_GAMEPAD_BUTTON_B ::PPGE::GamepadButtonCode::ButtonB
#define PPGE_GAMEPAD_BUTTON_X ::PPGE::GamepadButtonCode::ButtonX
#define PPGE_GAMEPAD_BUTTON_Y ::PPGE::GamepadButtonCode::ButtonY
#define PPGE_GAMEPAD_BUTTON_LEFT_BUMPER ::PPGE::GamepadButtonCode::ButtonLeftBumper
#define PPGE_GAMEPAD_BUTTON_RIGHT_BUMPER ::PPGE::GamepadButtonCode::ButtonRightBumper
#define PPGE_GAMEPAD_BUTTON_BACK ::PPGE::GamepadButtonCode::ButtonBack
#define PPGE_GAMEPAD_BUTTON_START ::PPGE::GamepadButtonCode::ButtonStart
#define PPGE_GAMEPAD_BUTTON_GUIDE ::PPGE::GamepadButtonCode::ButtonGuide
#define PPGE_GAMEPAD_BUTTON_LEFT_THUMB ::PPGE::GamepadButtonCode::ButtonLeftThumb
#define PPGE_GAMEPAD_BUTTON_RIGHT_THUMB ::PPGE::GamepadButtonCode::ButtonRightThumb
#define PPGE_GAMEPAD_BUTTON_DPAD_UP ::PPGE::GamepadButtonCode::ButtonDpadUp
#define PPGE_GAMEPAD_BUTTON_DPAD_RIGHT ::PPGE::GamepadButtonCode::ButtonDpadRight
#define PPGE_GAMEPAD_BUTTON_DPAD_DOWN ::PPGE::GamepadButtonCode::ButtonDpadDown
#define PPGE_GAMEPAD_BUTTON_DPAD_LEFT ::PPGE::GamepadButtonCode::ButtonDpadLeft
#define PPGE_GAMEPAD_BUTTON_LAST ::PPGE::GamepadButtonCode::ButtonLast
#define PPGE_GAMEPAD_BUTTON_CROSS ::PPGE::GamepadButtonCode::ButtonCross
#define PPGE_GAMEPAD_BUTTON_CIRCLE ::PPGE::GamepadButtonCode::ButtonCircle
#define PPGE_GAMEPAD_BUTTON_SQUARE ::PPGE::GamepadButtonCode::ButtonSquare
#define PPGE_GAMEPAD_BUTTON_TRIANGLE ::PPGE::GamepadButtonCode::ButtonTriangle

#define PPGE_GAMEPAD_AXIS_LEFT_X ::PPGE::GamepadAxisCode::AxisLeftX
#define PPGE_GAMEPAD_AXIS_LEFT_Y ::PPGE::GamepadAxisCode::AxisLeftY
#define PPGE_GAMEPAD_AXIS_RIGHT_X ::PPGE::GamepadAxisCode::AxisRightX
#define PPGE_GAMEPAD_AXIS_RIGHT_Y ::PPGE::GamepadAxisCode::AxisRightY
#define PPGE_GAMEPAD_AXIS_LEFT_TRIGGER ::PPGE::GamepadAxisCode::AxisLeftTrigger
#define PPGE_GAMEPAD_AXIS_RIGHT_TRIGGER ::PPGE::GamepadAxisCode::AxisRightTrigger
#define PPGE_GAMEPAD_AXIS_LAST ::PPGE::GamepadAxisCode::AxisLast
