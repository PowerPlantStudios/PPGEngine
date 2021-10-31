#include "core/input.h"

#include "system/display_system.h"

namespace PPGE
{
bool Input::IsKeyPressed(const KeyCode code)
{
    return DisplaySystem::Get().IsKeyPressed(code);
}
bool Input::IsMouseButtonPressed(const MouseCode code)
{
    return DisplaySystem::Get().IsMouseButtonPressed(code);
}
float Input::GetMouseX()
{
    return DisplaySystem::Get().GetMouseX();
}
float Input::GetMouseY()
{
    return DisplaySystem::Get().GetMouseY();
}
} // namespace PPGE
