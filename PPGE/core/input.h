#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/input_codes.h"

namespace PPGE
{
class PPGE_API Input
{
  public:
    static bool IsKeyPressed(const KeyCode code);
    static bool IsMouseButtonPressed(const MouseCode code);
    static float GetMouseX();
    static float GetMouseY();
};
} // namespace PPGE
