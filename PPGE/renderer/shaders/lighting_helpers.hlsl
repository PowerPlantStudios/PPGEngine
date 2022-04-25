/*  Helpers for lighting calculations
*/

#ifndef PPGE_LIGHTING_HELPERS
#define PPGE_LIGHTING_HELPERS

#include "common_buffers.hlsl"
#include "common_defines.hlsl"
#include "material_helpers.hlsl"

#define LIGHT_IS_DIRECTIONAL   uint(1U << 0)
#define LIGHT_IS_POINT         uint(1U << 1)
#define LIGHT_IS_SPOT          uint(1U << 2)
#define LIGHT_CAN_CAST_SHADOW  uint(1U << 3)

bool light_is_directional()  { return g_light_options & LIGHT_IS_DIRECTIONAL;  }
bool light_is_point()        { return g_light_options & LIGHT_IS_POINT;        }
bool light_is_spot()         { return g_light_options & LIGHT_IS_SPOT;         }
bool light_can_cast_shadow() { return g_light_options & LIGHT_CAN_CAST_SHADOW; }

struct LitColor
{
    float3 diffuse;
    float3 specular;
};

struct Light
{
    float3 color;
    float3 light_to_fragment;
    float  light_to_fragment_distance;
    float  attenuation;

    bool IsInRange() { return light_to_fragment_distance < g_light_range; }

    void Initialize(float3 frag_pos)
    {
        color = g_light_color * g_light_intensity;

        [flatten]
        if (!light_is_directional())
        {
            light_to_fragment = frag_pos - g_light_position;
            light_to_fragment_distance = length(light_to_fragment);
            light_to_fragment = normalize(light_to_fragment);
        }
        else
        {
            light_to_fragment_distance = PLUS_INF;
            light_to_fragment = normalize(g_light_direction);
        }

        [flatten]
        // No attenuation for directional light
        if (light_is_directional())
        {
            attenuation = 1.0f;
        }
        // Distance attenuation for point light
        else if (light_is_point())
        {
            attenuation = 1.0f / dot(g_light_dist_attenuation, 
                float3(1.0f, light_to_fragment_distance, light_to_fragment_distance * light_to_fragment_distance));
        }
        // Distance and angle attenuation for spot light
        else if (light_is_spot())
        {
            float dist_attenuation = 1.0f / dot(g_light_dist_attenuation, 
                float3(1.0f, light_to_fragment_distance, light_to_fragment_distance * light_to_fragment_distance));

            float angle_attenuation = 0.0f;

            float theta = acos(dot(normalize(g_light_direction), light_to_fragment));
            float outer_cutoff = radians(min(g_light_angle_attenuation.x, 89.5f));
            float inner_cutoff = min(radians(max(g_light_angle_attenuation.y, 0)), outer_cutoff);

            [flatten]
            if (theta < inner_cutoff)
            {
                angle_attenuation = 1.0f;
            }
            else if (theta < outer_cutoff)
            {
                float decay_rate = max(g_light_angle_attenuation.z, 0.0f);
                float cos_x = cos(theta - inner_cutoff);
                float cos_cufoff = cos(outer_cutoff - inner_cutoff);

                angle_attenuation = pow((1.0f - (1.0f - cos_x) / (1.0f - cos_cufoff)), decay_rate);
            }

            attenuation = angle_attenuation * dist_attenuation;
        }
    }

    LitColor ShadeFragment(Fragment frag)
    {
        LitColor lit_color;
        lit_color.diffuse  = float3(0.0f, 0.0f, 0.0f);
        lit_color.specular = float3(0.0f, 0.0f, 0.0f);

        if (!light_is_directional() && !IsInRange())
            return lit_color;

        float n_dot_l = dot(frag.normal, -light_to_fragment);
        
        [flatten]
        if (n_dot_l > 0.0f)
        {
            lit_color.diffuse = n_dot_l * (frag.albedo * color) * attenuation;

            float3 h = normalize(-light_to_fragment + frag.to_eye);
            float n_dot_h = dot(frag.normal, h);
            lit_color.specular = pow(saturate(n_dot_h), frag.shininess) * (frag.specular * color) * attenuation;
        }

        return lit_color;
    }
};

#endif // PPGE_LIGHTING_HELPERS
