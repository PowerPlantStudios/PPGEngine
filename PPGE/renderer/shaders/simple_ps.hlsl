cbuffer g_Color  : register(b0)
{
    float4 diffuse;
    float4 specular;
    bool is_textureless_diffuse;
    bool is_textureless_speculer;
};

Texture2D g_diffuse : register(t0);

SamplerState g_sampler : register(s0);

struct FragmentIn
{
	float4 PosH    : SV_POSITION;
	float3 PosL    : POSITION;
    float3 Normal  : NORMAL; 
    float2 Texture : TEXTURE;
    float3 EyePos  : VECTOR0;
};

float4 main(FragmentIn pin) : SV_Target
{
    float3 l_lightColor    = float3(0.85f, 0.8f, 1.0f);
    float  l_ambient       = 0.45;
    float3 l_lightPos      = float3(1.0f, 1.0f, 1.0f);
    float  l_specularPower = 24.0f;

    
    float3 lightDir = normalize(l_lightPos - pin.PosL);

    float ambient = l_ambient;
    float diff    = saturate(dot(pin.Normal, -lightDir));
    float spec    = pow(saturate(dot(reflect(lightDir, pin.Normal), 
                        normalize(pin.EyePos - pin.PosL))), l_specularPower);
    
    float3 lightColor = saturate(spec + diff + ambient) * l_lightColor;
    float3 texColor   = g_diffuse.Sample(g_sampler, pin.Texture).rgb;
    
    return float4(lightColor  * texColor, 1.0f);
}
