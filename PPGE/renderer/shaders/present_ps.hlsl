/*  PS that copies pixels from bound color texture
    into the currently bound render target
*/

Texture2D g_color_buffer : register(t0);
SamplerState g_sampler   : register(s0);

float4 main_PS(in float4 PosH : SV_POSITION, in float2 TexCoord0 : TEXTURE0) : SV_Target0
{
    return g_color_buffer.Sample(g_sampler, TexCoord0);
}
