#include "core/input.h"

#include "system/display_system.h"

namespace PPGE
{
std::tuple<float, float> GetMousePosition()
{
    POINT mouse_pos{0};
    GetCursorPos(&mouse_pos);
    ScreenToClient(static_cast<HWND>(DisplaySystem::Get().GetNativeDisplayPtr()), &mouse_pos);
    return {static_cast<float>(mouse_pos.x), static_cast<float>(mouse_pos.y)};
}

bool Input::IsKeyPressed(const KeyCode code)
{
    return GetKeyState(static_cast<int>(code)) & 0x8000;
}

bool Input::IsMouseButtonPressed(const MouseCode code)
{
    return GetKeyState(static_cast<int>(code)) & 0x8000;
}

float Input::GetMouseX()
{
    auto [x, y] = GetMousePosition();
    return x;
}

float Input::GetMouseY()
{
    auto [x, y] = GetMousePosition();
    return y;
}
} // namespace PPGE