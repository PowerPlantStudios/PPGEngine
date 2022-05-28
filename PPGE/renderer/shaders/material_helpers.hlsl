/*  Helpers for material calculations
*/

#ifndef PPGE_MATERIAL_HELPERS
#define PPGE_MATERIAL_HELPERS

#include "common_buffers.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "renderer_helpers.hlsl"

#define IS_ALBEDO_MAP_BOUND    uint(1U << 0)
#define IS_SPECULAR_MAP_BOUND  uint(1U << 1)
#define IS_NORMAL_MAP_BOUND    uint(1U << 2)

bool material_is_albedo_map_bound()    { return g_material_options & IS_ALBEDO_MAP_BOUND;   }
bool material_is_specular_map_bound()  { return g_material_options & IS_SPECULAR_MAP_BOUND; }
bool material_is_normal_map_bound()    { return g_material_options & IS_NORMAL_MAP_BOUND;   }

struct Fragment
{
    float  alpha;
    float3 albedo;
    float  shininess;
    float3 specular;
    float3 normal;
    float3 vertex_normal;
    float  depth;
    float3 position;
    float  to_eye_distance;
    float3 to_eye;

    float3 SampleNormalMap(float3 in_normal, float3 in_tangent, float2 in_uv)
    {
        float3 normal_map_value = g_texture_material_normal.Sample(g_sampler_anisotropic, in_uv).rgb;
        normal_map_value = 2.0f * normal_map_value - 1.0f;

        float3 N = normalize(in_normal);
        float3 T = normalize(in_tangent - dot(in_tangent, N) * N);
        float3 B = cross(N, T);

        float3x3 TBN = float3x3(T, B, N);

        return normalize(mul(normal_map_value, TBN));
    }

    void Initialize(float3 in_position, float3 in_normal, float3 in_tangent, float2 in_uv, float in_depth)
    {
        [flatten]
        if (material_is_albedo_map_bound())
        {
            albedo = g_texture_material_albedo.Sample(g_sampler_anisotropic, in_uv).rgb;
            alpha  = g_texture_material_albedo.Sample(g_sampler_anisotropic, in_uv).a;
        }
        else
        {
            albedo = g_albedo_color.rgb;
            alpha  = g_albedo_color.a;
        }

        shininess = g_specular_color.a;

        [flatten]
        if (material_is_specular_map_bound())
        {
            specular = g_texture_material_specular.Sample(g_sampler_anisotropic, in_uv).rgb;
        }
        else
        {
            specular = g_specular_color.rgb;
        }

        [flatten]
        if (renderer_is_normal_map_enabled() && material_is_normal_map_bound())
            normal = SampleNormalMap(in_normal, in_tangent, in_uv);
        else
            normal = normalize(in_normal);

        depth           = in_depth;
        vertex_normal   = normalize(in_normal);
        position        = in_position;
        to_eye          = g_cameraPosition - in_position;
        to_eye_distance = length(to_eye);
        to_eye          = normalize(to_eye);
    }
};

#endif // PPGE_MATERIAL_HELPERS
