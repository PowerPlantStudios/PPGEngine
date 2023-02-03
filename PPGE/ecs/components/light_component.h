#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "rhi/device_context.h"
#include "system/logger_system.h"

namespace PPGE
{
class PPGETextureView;

struct LightComponent
{
    enum class LightType : uint8_t
    {
        DIRECTIONAL = 0,
        POINT = 1,
        SPOT = 2
    };

    struct ShadowMapDSV
    {
        std::shared_ptr<PPGETextureView> dsv;
        Viewport viewport;
    };

    LightComponent(LightType light_type, bool cast_shadow = true) : type(light_type), can_cast_shadow(cast_shadow)
    {
        // For point light max number of dsv required is 6
        // For directional light number of cascades is 4
        shadow_map_dsv_array.reserve(6);

        Initialize();
    }

    // Color of the light (Only RGB components are taken into account)
    Math::Color color = PPGE::Math::Color{1.0f, 1.0f, 1.0f, 1.0f};
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

    bool CanCastShadow() const
    {
        return can_cast_shadow;
    }

    void SetCastShadow(bool cast_shadow)
    {
        if (can_cast_shadow == cast_shadow)
            return;

        can_cast_shadow = cast_shadow;

        if (can_cast_shadow)
            CreateShadowMapBuffers();
    }

    LightType GetLightType() const
    {
        return type;
    }

    void SetLightType(LightType ligt_type)
    {
        if (type == ligt_type)
            return;

        type = ligt_type;

        Initialize();
    }

    size_t GetShadowMapViewCount() const
    {
        if (CanCastShadow())
            return shadow_map_dsv_array.size();
        return 0;
    }

    Math::Matrix GetProj() const
    {
        if (CanCastShadow())
            return proj;
#if !defined(PPGE_PLATFORM_APPLE)
        return Math::Matrix::Identity;
#else
        return Math::Identity();
#endif
    }

    ShadowMapDSV GetShadowMapDSViewAt(size_t index) const
    {
        PPGE_ASSERT(index < GetShadowMapViewCount(), "Shadow map view index exceeds the maximum.");
        return shadow_map_dsv_array[index];
    }

    std::shared_ptr<PPGETextureView> GetShadowMapSRView() const
    {
        return shadow_map_srv;
    }

  private:
    LightType type = LightType::DIRECTIONAL;

    bool can_cast_shadow = false;

    std::vector<ShadowMapDSV> shadow_map_dsv_array;

    std::shared_ptr<PPGETextureView> shadow_map_srv;

    Math::Matrix proj;

    void Initialize()
    {
        if (can_cast_shadow)
        {
            CreateShadowMapBuffers();

            if (type == LightType::DIRECTIONAL)
            {
#if !defined(PPGE_PLATFORM_APPLE)
                proj = Math::Matrix::CreateOrthographic(16.0f, 16.0f, -16.0f, 16.0f);
#else
                proj = Math::CreateOrthographic(16.0f, 16.0f, -16.0f, 16.0f);
#endif
            }
            else
            {
                const float fov =
                    type == LightType::POINT ? 1.57079632679f : (2.0f * spot_cutoff_angle) / 180.0f * 3.14159265359f;

                constexpr uint32_t width = 2048;
                constexpr uint32_t height = 2048;
                constexpr float aspect_ratio = static_cast<float>(width) / static_cast<float>(height);

                constexpr float near_plane = 0.1f;
                const float far_plane = range > 0.1f ? range : 0.1f;
#if !defined(PPGE_PLATFORM_APPLE)
                proj = Math::Matrix::CreatePerspectiveFieldOfView(fov, aspect_ratio, near_plane, far_plane);
#else
                proj = Math::CreatePerspectiveFieldOfView(fov, aspect_ratio, near_plane, far_plane);
#endif
            }
        }
    }

    void CreateShadowMapBuffers();
};
} // namespace PPGE
