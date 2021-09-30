#pragma once

#include "core/defines.h"

#if defined(DISPLAY_API_GLFW)
#include "platform/glfw/gamepad_codes_glfw.h"
#include "platform/glfw/joystick_codes_glfw.h"
#include "platform/glfw/key_codes_glfw.h"
#include "platform/glfw/mouse_codes_glfw.h"
#elif defined(DISPLAY_API_WIN32)

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