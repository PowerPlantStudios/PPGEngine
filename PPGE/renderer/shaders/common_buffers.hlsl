/*  Common constant buffers
*/

#ifndef PPGE_COMMON_BUFFERS
#define PPGE_COMMON_BUFFERS

cbuffer cb_Renderer : register(b0)
{
    uint g_renderer_options : packoffset(c0);
} 

cbuffer cb_PerFrame : register(b1)
{
    float4x4 g_view            : packoffset(c0);
    float4x4 g_viewInv         : packoffset(c4);
    float4x4 g_proj            : packoffset(c8);
    float4x4 g_projInv         : packoffset(c12);
    float4x4 g_viewProj        : packoffset(c16);
    float4x4 g_viewProjInv     : packoffset(c20);

    float3   g_cameraPosition  : packoffset(c24);
    float3   g_cameraDirection : packoffset(c25);
};

cbuffer cb_PerDraw : register(b2)
{
    float4x4 g_world            : packoffset(c0);
    float4x4 g_worldInvTran     : packoffset(c4);

    float4   g_albedo_color     : packoffset(c8);
    float4   g_specular_color   : packoffset(c9);

    uint     g_material_options : packoffset(c10.x);
    uint     g_entity_id        : packoffset(c10.y);
};

cbuffer cb_Light : register(b3)
{
    float4x4 g_light_viewProj          : packoffset(c0);
    float3   g_light_position          : packoffset(c4);
    float3   g_light_direction         : packoffset(c5);
    uint     g_light_options           : packoffset(c6);
    float3   g_light_color             : packoffset(c7);    // Valid for all light types
    float    g_light_intensity         : packoffset(c7.w);  // Valid for all light types
    float3   g_light_dist_attenuation  : packoffset(c8);    // Only valid for point and spot light
    float    g_light_range             : packoffset(c8.w);  // Only valid for point and spot light
    float3   g_light_angle_attenuation : packoffset(c9);    // Only valid for spot light (theta_max, theta_inner, decay_rate)
};

#endif // PPGE_COMMON_BUFFERS
