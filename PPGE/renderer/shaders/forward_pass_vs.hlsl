/*  Forwardpass Renderer VS
*/

#include "color_helpers.hlsl"
#include "common_buffers.hlsl"
#include "common_layouts.hlsl"

PixelIn main_VS(VertexIn vin)
{
	PixelIn vout;

    // World position
    vout.PosL = mul(float4(vin.PosL, 1.0), g_world).xyz;

    // Homogenous device position
    vout.PosH = mul(float4(vout.PosL, 1.0), g_viewProj);

    // World normal
    vout.Normal = normalize(mul(float4(vin.Normal, 0.0f), g_worldInvTran).xyz);

    // World tangent
    vout.Tangent = normalize(mul(float4(vin.Tangent, 0.0f), g_world).xyz);

    // TexCoord
    vout.TexCoord0 = vin.TexCoord0;
    
    // Vertex color
	vout.Color = Decode32bitRGBA(vin.Color); 
    
    return vout;
}
