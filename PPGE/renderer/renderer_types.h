#pragma once
#include "PPGEpch.h"

#include "core/defines.h"
#include "core/ppge_enum.h"
#include "rhi/pipeline_state.h"

namespace PPGE
{
struct VertexPosColor
{
    float px, py, pz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
};

struct VertexPosNorColor
{
    float px, py, pz;
    float nx, ny, nz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
};

struct StandardVertex
{
    float px, py, pz;
    float nx, ny, nz;
    float tx, ty, tz;
    float btx, bty, btz;
    union {
        struct
        {
            uint8_t r;
            uint8_t b;
            uint8_t g;
            uint8_t a;
        } channel;
        uint32_t color;
    };
    float u1, v1, u2, v2, u3, v3;
};

enum class RendererOptions : uint32_t
{
    NONE = 0,
    ENABLE_NORMAL_MAP = 1 << 0
};
PPGE_ENUM_OPERATORS(RendererOptions);

enum class LightOptions : uint32_t
{
    NONE = 0,
    TYPE_DIRECTIONAL_LIGHT = 1 << 0,
    TYPE_POINT_LIGHT = 1 << 1,
    TYPE_SPOT_LIGHT = 1 << 2,
    CAN_CAST_SHADOW = 1 << 3
};
PPGE_ENUM_OPERATORS(LightOptions);

enum class MaterialOptions : uint32_t
{
    NONE = 0,
    ALBEDO_MAP_BOUND = 1 << 0,
    SPECULAR_MAP_BOUND = 1 << 1,
    NORMAL_MAP_BOUND = 1 << 2,
};
PPGE_ENUM_OPERATORS(MaterialOptions);

struct alignas(16) CbRenderer
{
    RendererOptions renderer_options;
    uint32_t pad[3];
    Math::Vector2 render_resolution;
    float shadowmap_resolution;
};
constexpr const char *CbRendererOptionsName = "RendererOptions";

struct alignas(16) CbPerFrame
{
    Math::Matrix view;
    Math::Matrix view_inverse;
    Math::Matrix proj;
    Math::Matrix proj_inverse;
    Math::Matrix view_proj;
    Math::Matrix view_proj_inverse;
    Math::Vector4 camera_position;
    Math::Vector4 camera_direction;
};
constexpr const char *CbPerFrameResourceName = "PerFrameConstantBuffer";

struct alignas(16) CbPerDraw
{
    Math::Matrix world;
    Math::Matrix world_inverse_transpose;

    Math::Color albedo_color;
    Math::Color specular_color;

    MaterialOptions material_options;
    uint32_t entity_id;
    uint32_t pad[2];
};
constexpr const char *CbPerDrawResourceName = "PerDrawConstantBuffer";

struct alignas(16) CbLight
{
    Math::Matrix view_proj[6];
    Math::Vector3 position;
    uint32_t pad1;
    Math::Vector3 direction;
    uint32_t pad2;
    LightOptions light_options;
    uint32_t pad3[3];
    Math::Color color_intensity;
    Math::Vector4 dist_attenuation_range;
    Math::Vector4 angle_attenuation;
};
constexpr const char *CbLightResourceName = "LightConstBuffer";
} // namespace PPGE
