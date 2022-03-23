#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
struct PointLightComponents
{

};

struct DirectionalLightComponent
{
    PPGE::Math::Color color = PPGE::Math::Color(1.0, 1.0, 1.0);
    float intensity = 1.0f;
};

struct SpotLightComponent
{

};
} // namespace PPGE
