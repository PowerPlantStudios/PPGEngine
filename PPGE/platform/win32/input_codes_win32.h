#pragma once
#include "PPGEpch.h"

#include "core/input_codes.h"

namespace PPGE
{
inline constexpr std::array<uint16_t, static_cast<size_t>(KeyCode::PPGE_KEY_CODE_COUNT)> Win32_Key_Codes = {
    /*Space*/ VK_SPACE,
    /*Apostrophe*/ 39,
    /*Comma*/ VK_OEM_COMMA,
    /*Minus*/ VK_OEM_MINUS,
    /*Period*/ VK_OEM_PERIOD,
    /*Slash*/ VK_OEM_2,
    /*D0*/ 48,
    /*D1*/ 49,
    /*D2*/ 50,
    /*D3*/ 51,
    /*D4*/ 52,
    /*D5*/ 53,
    /*D6*/ 54,
    /*D7*/ 55,
    /*D8*/ 56,
    /*D9*/ 57,
    /*Semicolon*/ VK_OEM_1,
    /*Equal*/ 61,
    /*A*/ 65,
    /*B*/ 66,
    /*C*/ 67,
    /*D*/ 68,
    /*E*/ 69,
    /*F*/ 70,
    /*G*/ 71,
    /*H*/ 72,
    /*I*/ 73,
    /*J*/ 74,
    /*K*/ 75,
    /*L*/ 76,
    /*M*/ 77,
    /*N*/ 78,
    /*O*/ 79,
    /*P*/ 80,
    /*Q*/ 81,
    /*R*/ 82,
    /*S*/ 83,
    /*T*/ 84,
    /*U*/ 85,
    /*V*/ 86,
    /*W*/ 87,
    /*X*/ 88,
    /*Y*/ 89,
    /*Z*/ 90,
    /*LeftBracket*/ 91,
    /*Backslash*/ 92,
    /*RightBracket*/ 93,
    /*GraveAccent*/ 96,
    /*World1*/ 161,
    /*World2*/ 162,
    /*Escape*/ VK_ESCAPE,
    /*Enter*/ VK_RETURN,
    /*Tab*/ VK_TAB,
    /*Backspace*/ VK_CLEAR,
    /*Insert*/ VK_INSERT,
    /*Delete*/ VK_DELETE,
    /*Right*/ VK_RIGHT,
    /*Left*/ VK_LEFT,
    /*Down*/ VK_DOWN,
    /*Up*/ VK_UP,
    /*PageUp*/ 266,
    /*PageDown*/ 267,
    /*Home*/ VK_HOME,
    /*End*/ VK_END,
    /*CapsLock*/ 280,
    /*ScrollLock*/ VK_SCROLL,
    /*NumLock*/ VK_NUMLOCK,
    /*PrintScreen*/ 283,
    /*Pause*/ 284,
    /*F1*/ VK_F1,
    /*F2*/ VK_F2,
    /*F3*/ VK_F3,
    /*F4*/ VK_F4,
    /*F5*/ VK_F5,
    /*F6*/ VK_F6,
    /*F7*/ VK_F7,
    /*F8*/ VK_F8,
    /*F9*/ VK_F9,
    /*F10*/ VK_F10,
    /*F11*/ VK_F11,
    /*F12*/ VK_F12,
    /*F13*/ VK_F13,
    /*F14*/ VK_F14,
    /*F15*/ VK_F15,
    /*F16*/ VK_F16,
    /*F17*/ VK_F17,
    /*F18*/ VK_F18,
    /*F19*/ VK_F19,
    /*F20*/ VK_F20,
    /*F21*/ VK_F21,
    /*F22*/ VK_F22,
    /*F23*/ VK_F23,
    /*F24*/ VK_F24,
    /*F25*/ 0,
    /*KP0*/ VK_NUMPAD0,
    /*KP1*/ VK_NUMPAD1,
    /*KP2*/ VK_NUMPAD2,
    /*KP3*/ VK_NUMPAD3,
    /*KP4*/ VK_NUMPAD4,
    /*KP5*/ VK_NUMPAD5,
    /*KP6*/ VK_NUMPAD6,
    /*KP7*/ VK_NUMPAD7,
    /*KP8*/ VK_NUMPAD8,
    /*KP9*/ VK_NUMPAD9,
    /*KPDecimal*/ VK_DECIMAL,
    /*KPDivide*/ VK_DIVIDE,
    /*KPMultiply*/ VK_MULTIPLY,
    /*KPSubtract*/ VK_SUBTRACT,
    /*KPAdd*/ VK_ADD,
    /*KPEnter*/ 335,
    /*KPEqual*/ 336,
    /*LeftShift*/ 340,
    /*LeftControl*/ 341,
    /*LeftAlt*/ 342,
    /*LeftSuper*/ 343,
    /*RightShift*/ 344,
    /*RightControl*/ 345,
    /*RightAlt*/ 346,
    /*RightSuper*/ 347,
    /*Menu*/ 348,
};

inline constexpr std::array<uint16_t, static_cast<size_t>(MouseCode::PPGE_MOUSE_CODE_COUNT)> Win32_Mouse_Codes = {
    /*Button0*/ VK_LBUTTON,
    /*Button1*/ VK_RBUTTON,
    /*Button2*/ VK_MBUTTON,
    /*Button3*/ VK_XBUTTON1,
    /*Button4*/ VK_XBUTTON2,
    /*Button5*/ VK_LBUTTON,
    /*Button6*/ VK_RBUTTON,
    /*Button7*/ VK_MBUTTON,
    /*ButtonLeft*/ VK_LBUTTON,
    /*ButtonRight*/ VK_RBUTTON,
    /*ButtonMiddle*/ VK_MBUTTON,
};

inline constexpr std::array<uint16_t, static_cast<size_t>(JoystickCode::PPGE_JOYSTICK_CODE_COUNT)>
    Win32_Joystick_Codes = {
        /*Joystick0*/ 0,
        /*Joystick1*/ 1,
        /*Joystick2*/ 2,
        /*Joystick3*/ 3,
        /*Joystick4*/ 4,
        /*Joystick5*/ 5,
        /*Joystick6*/ 6,
        /*Joystick7*/ 7,
        /*Joystick8*/ 8,
        /*Joystick9*/ 9,
        /*Joystick10*/ 10,
        /*Joystick11*/ 11,
        /*Joystick12*/ 12,
        /*Joystick13*/ 13,
        /*Joystick14*/ 14,
        /*Joystick15*/ 15,
};

inline constexpr std::array<uint16_t, static_cast<size_t>(GamepadCode::PPGE_GAMEPAD_CODE_COUNT)> Win32_Gamepad_Codes = {
    /*ButtonA*/ 0,
    /*ButtonB*/ 1,
    /*ButtonX*/ 2,
    /*ButtonY*/ 3,
    /*ButtonLeftBumper*/ 4,
    /*ButtonRightBumper*/ 5,
    /*ButtonBack*/ 6,
    /*ButtonStart*/ 7,
    /*ButtonGuide*/ 8,
    /*ButtonLeftThumb*/ 9,
    /*ButtonRightThumb*/ 10,
    /*ButtonDpadUp*/ 11,
    /*ButtonDpadRight*/ 12,
    /*ButtonDpadDown*/ 13,
    /*ButtonDpadLeft*/ 14,
    /*ButtonCross*/ 0,
    /*ButtonCircle*/ 1,
    /*ButtonSquare*/ 2,
    /*ButtonTriangle*/ 3,
    /*AxisLeftX*/ 0,
    /*AxisLeftY*/ 1,
    /*AxisRightX*/ 2,
    /*AxisRightY*/ 3,
    /*AxisLeftTrigger*/ 4,
    /*AxisRightTrigger*/ 5,
};
} // namespace PPGE
