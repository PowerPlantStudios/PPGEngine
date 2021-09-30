#pragma once
#include "PPGEpch.h"

#include <GLFW/glfw3.h>

#include "core/input.h"
#include "system/display_system.h"

namespace PPGE
{
std::tuple<float, float> GetMousePosition()
{
    auto *window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    double xpos, ypos;
    glfwGetCursorPos(window_ptr, &xpos, &ypos);
    return {xpos, ypos};
}

bool Input::IsKeyPressed(const KeyCode code)
{
    auto *window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    auto state = glfwGetKey(window_ptr, static_cast<int>(code));
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonPressed(const MouseCode code)
{
    auto *window_ptr = static_cast<GLFWwindow *>(DisplaySystem::Get().GetNativeDisplayPtr());
    auto state = glfwGetMouseButton(window_ptr, static_cast<int>(code));
    return state == GLFW_PRESS;
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