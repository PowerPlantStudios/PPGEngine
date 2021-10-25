#pragma once
#include "PPGEpch.h"

#include "core/defines.h"

#if defined(DISPLAY_API_GLFW)
#include "platform/glfw/gamepad_codes_glfw.h"
#include "platform/glfw/joystick_codes_glfw.h"
#include "platform/glfw/key_codes_glfw.h"
#include "platform/glfw/mouse_codes_glfw.h"
#elif defined(DISPLAY_API_WIN32)
#include "platform/win32/gamepad_codes_win32.h"
#include "platform/win32/joystick_codes_win32.h"
#include "platform/win32/key_codes_win32.h"
#include "platform/win32/mouse_codes_win32.h"
#endif

namespace PPGE
{
class PPGE_API Input
{
  public:
    static bool IsKeyPressed(const KeyCode code);
    static bool IsMouseButtonPressed(const MouseCode code);
    static float GetMouseX();
    static float GetMouseY();

  private:
};
} // namespace PPGE