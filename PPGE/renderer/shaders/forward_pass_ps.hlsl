/*  Forwardpass Renderer PS
*/

#include "common_buffers.hlsl"
#include "common_layouts.hlsl"
#include "common_samplers.hlsl"
#include "common_textures.hlsl"
#include "lighting_helpers.hlsl"
#include "material_helpers.hlsl"

float4 ambient_pass_PS(PixelIn pin) : SV_Target
{
    float3 albedo = float3(0.0f, 0.0f, 0.0f);
    float  alpha = 1.0f;

    [flatten]
    if (material_is_albedo_map_bound())
    {
        albedo = g_texture_material_albedo.Sample(g_sampler_anisotropic_wrap, pin.TexCoord0).rgb;
        alpha  = g_texture_material_albedo.Sample(g_sampler_anisotropic_wrap, pin.TexCoord0).a;
    }
    else
    {
        albedo = g_albedo_color.rgb;
        alpha  = g_albedo_color.a;
    }

    return float4(0.05f * albedo * float3(1.0f, 1.0f, 1.0f), alpha);
}


float4 light_pass_PS(PixelIn pin) : SV_Target
{
    Light light;
    light.Initialize(pin.PosL);

    Fragment frag;
    frag.Initialize(pin.PosL, pin.Normal, pin.Tangent, pin.TexCoord0, pin.PosH.z / pin.PosH.w);

    LitColor lit_color = light.ShadeFragment(frag);

    return saturate(float4(lit_color.diffuse + lit_color.specular, frag.alpha));
}
