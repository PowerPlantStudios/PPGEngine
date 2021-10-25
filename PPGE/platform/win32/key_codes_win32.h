#pragma once
#include "PPGEpch.h"

namespace PPGE
{
enum class KeyCode : uint16_t
{
    Space = VK_SPACE,
    Apostrophe = 39,        /* ' */
    Comma = VK_OEM_COMMA,   /* , */
    Minus = VK_OEM_MINUS,   /* - */
    Period = VK_OEM_PERIOD, /* . */
    Slash = VK_OEM_2,       /* / */

    D0 = 48, /* 0 */
    D1 = 49, /* 1 */
    D2 = 50, /* 2 */
    D3 = 51, /* 3 */
    D4 = 52, /* 4 */
    D5 = 53, /* 5 */
    D6 = 54, /* 6 */
    D7 = 55, /* 7 */
    D8 = 56, /* 8 */
    D9 = 57, /* 9 */

    Semicolon = VK_OEM_1, /* ; */
    Equal = 61,           /* = */

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket = 91,  /* [ */
    Backslash = 92,    /* \ */
    RightBracket = 93, /* ] */
    GraveAccent = 96,  /* ` */

    World1 = 161, /* non-US #1 */
    World2 = 162, /* non-US #2 */

    /* Function keys */
    Escape = VK_ESCAPE,
    Enter = VK_RETURN,
    Tab = VK_TAB,
    Backspace = VK_CLEAR,
    Insert = VK_INSERT,
    Delete = VK_DELETE,
    Right = VK_RIGHT,
    Left = VK_LEFT,
    Down = VK_DOWN,
    Up = VK_UP,
    PageUp = 266,
    PageDown = 267,
    Home = VK_HOME,
    End = VK_END,
    CapsLock = 280,
    ScrollLock = VK_SCROLL,
    NumLock = VK_NUMLOCK,
    PrintScreen = 283,
    Pause = 284,
    F1 = VK_F1,
    F2 = VK_F2,
    F3 = VK_F3,
    F4 = VK_F4,
    F5 = VK_F5,
    F6 = VK_F6,
    F7 = VK_F7,
    F8 = VK_F8,
    F9 = VK_F9,
    F10 = VK_F10,
    F11 = VK_F11,
    F12 = VK_F12,
    F13 = VK_F13,
    F14 = VK_F14,
    F15 = VK_F15,
    F16 = VK_F16,
    F17 = VK_F17,
    F18 = VK_F18,
    F19 = VK_F19,
    F20 = VK_F20,
    F21 = VK_F21,
    F22 = VK_F22,
    F23 = VK_F23,
    F24 = VK_F24,
    F25 = 0,

    /* Keypad */
    KP0 = VK_NUMPAD0,
    KP1 = VK_NUMPAD1,
    KP2 = VK_NUMPAD2,
    KP3 = VK_NUMPAD3,
    KP4 = VK_NUMPAD4,
    KP5 = VK_NUMPAD5,
    KP6 = VK_NUMPAD6,
    KP7 = VK_NUMPAD7,
    KP8 = VK_NUMPAD8,
    KP9 = VK_NUMPAD9,
    KPDecimal = VK_DECIMAL,
    KPDivide = VK_DIVIDE,
    KPMultiply = VK_MULTIPLY,
    KPSubtract = VK_SUBTRACT,
    KPAdd = VK_ADD,
    KPEnter = 335,
    KPEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348
};

inline std::ostream &operator<<(std::ostream &os, KeyCode keycode)
{
    os << static_cast<int32_t>(keycode);
    return os;
}
} // namespace PPGE

// From glfw3.h
#define PPGE_KEY_SPACE ::PPGE::KeyCode::Space
#define PPGE_KEY_APOSTROPHE ::PPGE::KeyCode::Apostrophe /* ' */
#define PPGE_KEY_COMMA ::PPGE::KeyCode::Comma /* , */
#define PPGE_KEY_MINUS ::PPGE::KeyCode::Minus /* - */
#define PPGE_KEY_PERIOD ::PPGE::KeyCode::Period /* . */
#define PPGE_KEY_SLASH ::PPGE::KeyCode::Slash /* / */
#define PPGE_KEY_0 ::PPGE::KeyCode::D0
#define PPGE_KEY_1 ::PPGE::KeyCode::D1
#define PPGE_KEY_2 ::PPGE::KeyCode::D2
#define PPGE_KEY_3 ::PPGE::KeyCode::D3
#define PPGE_KEY_4 ::PPGE::KeyCode::D4
#define PPGE_KEY_5 ::PPGE::KeyCode::D5
#define PPGE_KEY_6 ::PPGE::KeyCode::D6
#define PPGE_KEY_7 ::PPGE::KeyCode::D7
#define PPGE_KEY_8 ::PPGE::KeyCode::D8
#define PPGE_KEY_9 ::PPGE::KeyCode::D9
#define PPGE_KEY_SEMICOLON ::PPGE::KeyCode::Semicolon /* ; */
#define PPGE_KEY_EQUAL ::PPGE::KeyCode::Equal /* = */
#define PPGE_KEY_A ::PPGE::KeyCode::A
#define PPGE_KEY_B ::PPGE::KeyCode::B
#define PPGE_KEY_C ::PPGE::KeyCode::C
#define PPGE_KEY_D ::PPGE::KeyCode::D
#define PPGE_KEY_E ::PPGE::KeyCode::E
#define PPGE_KEY_F ::PPGE::KeyCode::F
#define PPGE_KEY_G ::PPGE::KeyCode::G
#define PPGE_KEY_H ::PPGE::KeyCode::H
#define PPGE_KEY_I ::PPGE::KeyCode::I
#define PPGE_KEY_J ::PPGE::KeyCode::J
#define PPGE_KEY_K ::PPGE::KeyCode::K
#define PPGE_KEY_L ::PPGE::KeyCode::L
#define PPGE_KEY_M ::PPGE::KeyCode::M
#define PPGE_KEY_N ::PPGE::KeyCode::N
#define PPGE_KEY_O ::PPGE::KeyCode::O
#define PPGE_KEY_P ::PPGE::KeyCode::P
#define PPGE_KEY_Q ::PPGE::KeyCode::Q
#define PPGE_KEY_R ::PPGE::KeyCode::R
#define PPGE_KEY_S ::PPGE::KeyCode::S
#define PPGE_KEY_T ::PPGE::KeyCode::T
#define PPGE_KEY_U ::PPGE::KeyCode::U
#define PPGE_KEY_V ::PPGE::KeyCode::V
#define PPGE_KEY_W ::PPGE::KeyCode::W
#define PPGE_KEY_X ::PPGE::KeyCode::X
#define PPGE_KEY_Y ::PPGE::KeyCode::Y
#define PPGE_KEY_Z ::PPGE::KeyCode::Z
#define PPGE_KEY_LEFT_BRACKET ::PPGE::KeyCode::LeftBracket /* [ */
#define PPGE_KEY_BACKSLASH ::PPGE::KeyCode::Backslash /* \ */
#define PPGE_KEY_RIGHT_BRACKET ::PPGE::KeyCode::RightBracket /* ] */
#define PPGE_KEY_GRAVE_ACCENT ::PPGE::KeyCode::GraveAccent /* ` */
#define PPGE_KEY_WORLD_1 ::PPGE::KeyCode::World1 /* non-US #1 */
#define PPGE_KEY_WORLD_2 ::PPGE::KeyCode::World2 /* non-US #2 */

/* Function keys */
#define PPGE_KEY_ESCAPE ::PPGE::KeyCode::Escape
#define PPGE_KEY_ENTER ::PPGE::KeyCode::Enter
#define PPGE_KEY_TAB ::PPGE::KeyCode::Tab
#define PPGE_KEY_BACKSPACE ::PPGE::KeyCode::Backspace
#define PPGE_KEY_INSERT ::PPGE::KeyCode::Insert
#define PPGE_KEY_DELETE ::PPGE::KeyCode::Delete
#define PPGE_KEY_RIGHT ::PPGE::KeyCode::Right
#define PPGE_KEY_LEFT ::PPGE::KeyCode::Left
#define PPGE_KEY_DOWN ::PPGE::KeyCode::Down
#define PPGE_KEY_UP ::PPGE::KeyCode::Up
#define PPGE_KEY_PAGE_UP ::PPGE::KeyCode::PageUp
#define PPGE_KEY_PAGE_DOWN ::PPGE::KeyCode::PageDown
#define PPGE_KEY_HOME ::PPGE::KeyCode::Home
#define PPGE_KEY_END ::PPGE::KeyCode::End
#define PPGE_KEY_CAPS_LOCK ::PPGE::KeyCode::CapsLock
#define PPGE_KEY_SCROLL_LOCK ::PPGE::KeyCode::ScrollLock
#define PPGE_KEY_NUM_LOCK ::PPGE::KeyCode::NumLock
#define PPGE_KEY_PRINT_SCREEN ::PPGE::KeyCode::PrintScreen
#define PPGE_KEY_PAUSE ::PPGE::KeyCode::Pause
#define PPGE_KEY_F1 ::PPGE::KeyCode::F1
#define PPGE_KEY_F2 ::PPGE::KeyCode::F2
#define PPGE_KEY_F3 ::PPGE::KeyCode::F3
#define PPGE_KEY_F4 ::PPGE::KeyCode::F4
#define PPGE_KEY_F5 ::PPGE::KeyCode::F5
#define PPGE_KEY_F6 ::PPGE::KeyCode::F6
#define PPGE_KEY_F7 ::PPGE::KeyCode::F7
#define PPGE_KEY_F8 ::PPGE::KeyCode::F8
#define PPGE_KEY_F9 ::PPGE::KeyCode::F9
#define PPGE_KEY_F10 ::PPGE::KeyCode::F10
#define PPGE_KEY_F11 ::PPGE::KeyCode::F11
#define PPGE_KEY_F12 ::PPGE::KeyCode::F12
#define PPGE_KEY_F13 ::PPGE::KeyCode::F13
#define PPGE_KEY_F14 ::PPGE::KeyCode::F14
#define PPGE_KEY_F15 ::PPGE::KeyCode::F15
#define PPGE_KEY_F16 ::PPGE::KeyCode::F16
#define PPGE_KEY_F17 ::PPGE::KeyCode::F17
#define PPGE_KEY_F18 ::PPGE::KeyCode::F18
#define PPGE_KEY_F19 ::PPGE::KeyCode::F19
#define PPGE_KEY_F20 ::PPGE::KeyCode::F20
#define PPGE_KEY_F21 ::PPGE::KeyCode::F21
#define PPGE_KEY_F22 ::PPGE::KeyCode::F22
#define PPGE_KEY_F23 ::PPGE::KeyCode::F23
#define PPGE_KEY_F24 ::PPGE::KeyCode::F24
#define PPGE_KEY_F25 ::PPGE::KeyCode::F25

/* Keypad */
#define PPGE_KEY_KP_0 ::PPGE::KeyCode::KP0
#define PPGE_KEY_KP_1 ::PPGE::KeyCode::KP1
#define PPGE_KEY_KP_2 ::PPGE::KeyCode::KP2
#define PPGE_KEY_KP_3 ::PPGE::KeyCode::KP3
#define PPGE_KEY_KP_4 ::PPGE::KeyCode::KP4
#define PPGE_KEY_KP_5 ::PPGE::KeyCode::KP5
#define PPGE_KEY_KP_6 ::PPGE::KeyCode::KP6
#define PPGE_KEY_KP_7 ::PPGE::KeyCode::KP7
#define PPGE_KEY_KP_8 ::PPGE::KeyCode::KP8
#define PPGE_KEY_KP_9 ::PPGE::KeyCode::KP9
#define PPGE_KEY_KP_DECIMAL ::PPGE::KeyCode::KPDecimal
#define PPGE_KEY_KP_DIVIDE ::PPGE::KeyCode::KPDivide
#define PPGE_KEY_KP_MULTIPLY ::PPGE::KeyCode::KPMultiply
#define PPGE_KEY_KP_SUBTRACT ::PPGE::KeyCode::KPSubtract
#define PPGE_KEY_KP_ADD ::PPGE::KeyCode::KPAdd
#define PPGE_KEY_KP_ENTER ::PPGE::KeyCode::KPEnter
#define PPGE_KEY_KP_EQUAL ::PPGE::KeyCode::KPEqual

#define PPGE_KEY_LEFT_SHIFT ::PPGE::KeyCode::LeftShift
#define PPGE_KEY_LEFT_CONTROL ::PPGE::KeyCode::LeftControl
#define PPGE_KEY_LEFT_ALT ::PPGE::KeyCode::LeftAlt
#define PPGE_KEY_LEFT_SUPER ::PPGE::KeyCode::LeftSuper
#define PPGE_KEY_RIGHT_SHIFT ::PPGE::KeyCode::RightShift
#define PPGE_KEY_RIGHT_CONTROL ::PPGE::KeyCode::RightControl
#define PPGE_KEY_RIGHT_ALT ::PPGE::KeyCode::RightAlt
#define PPGE_KEY_RIGHT_SUPER ::PPGE::KeyCode::RightSuper
#define PPGE_KEY_MENU ::PPGE::KeyCode::Menu
