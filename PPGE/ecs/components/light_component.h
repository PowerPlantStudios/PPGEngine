#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "system/logger_system.h"

namespace PPGE
{
struct LightComponent
{
    enum class LightType : uint8_t
    {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2
    };

    LightComponent(LightType light_type)
        : type(light_type)
    {
    }

    LightType type = LightType::DIRECTIONAL;
    // Color of the light (Only RGB components are taken into account)
    Math::Color color = PPGE::Math::Color(1.0, 1.0, 1.0, 1.0f);
    // Intensity of the light
    float intensity = 1.0f;
    // Attenuation of light with increasing distance of the frament from the light source.
    // (Only valid for point and spot light components)
    // Light is attenuated by 1 / (a0 + a1 * d + a2 * d^2)
    // Each factors can be controlled independently
    // Constant factor attenuation
    float dist_attenuation_a0 = 1.0f;
    // inversely proportional to distance attenuation
    float dist_attenuation_a1 = 0.35f;
    // inversely proportional to distance square attenuation
    float dist_attenuation_a2 = 0.44f;
    // Cutoff range of the light. Fragments further away from this will not get affected 
    // by the light source. (Only valid for point and spot light components)
    float range = 13.0f;
    // Cutoff angle for spot light. (Has no efffect for other light types than spot light)
    float spot_cutoff_angle = 60.0f;
    // Inner cone angle for spot light. (Has no efffect for other light types than spot light)
    float spot_inner_cone_angle = 20.0f;
    // Decay rate for spot light between inner_cone_angle and cutoff_angle.
    // (Has no efffect for other light types than spot light)
    float spot_decay_rate = 8.0f;
};
} // namespace PPGE
