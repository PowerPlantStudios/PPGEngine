struct VertexPosNorColorIn
{
    float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Bitangent : BITANGENT;
    uint   Color     : COLOR;
    float2 TexCord0  : TEXTURE0;
    float2 TexCord1  : TEXTURE1;
    float2 TexCord2  : TEXTURE2;
};

struct VertexPosNorColorOut
{
    float4 PosH      : SV_POSITION;
    float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Bitangent : BITANGENT;
    float4 Color     : COLOR;
    float2 TexCord0  : TEXTURE0;
    float2 TexCord1  : TEXTURE1;
    float2 TexCord2  : TEXTURE2;
};

cbuffer cb_PerFrame  : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewProj;
    float3   cameraPosition;
};

cbuffer cb_PerObject : register(b1)
{
	float4x4 model;
    float4x4 modelInvTran;
};

VertexPosNorColorOut main_VS(VertexPosNorColorIn vin)
{
	VertexPosNorColorOut vout;

    // World position
    vout.PosL = mul(float4(vin.PosL, 1.0), model).xyz;

    // Homogenous device position
    vout.PosH = mul(float4(vout.PosL, 1.0), viewProj);

    // World normal
    vout.Normal = normalize(mul(float4(vin.Normal, 0.0f), modelInvTran));

    // TexCoord
    vout.TexCord0 = vin.TexCord0;
    
    // Vertex color
    float r = ((vin.Color >> 24) & 0xff) / 255.0f;
    float g = ((vin.Color >> 16) & 0xff) / 255.0f;
    float b = ((vin.Color >>  8) & 0xff) / 255.0f;
    float a = ((vin.Color      ) & 0xff) / 255.0f;
	vout.Color = float4(r, g, b, a); 
    
    return vout;
}

cbuffer cb_View  : register(b0)
{
    float3 eye_pos;
    float pad;
};

cbuffer cb_Color  : register(b1)
{
    float4 albedo_color;
    float4 specular_color;
    bool albedo_map_bound;
    bool specular_map_bound;
    bool normal_map_bound;
};

Texture2D g_albedo   : register(t0);
Texture2D g_specular : register(t1);
Texture2D g_normal   : register(t2);

SamplerState g_sampler : register(s0);

struct DirectionalLight
{
    float4 ambient;
    float4 diffuse;
    float4 specular;
    float3 direction;
    float pad;
};

struct Fragment
{
    float4 albedo;
    float4 specular;
    float4 normal;
    float4 toEye;
};

void CalculateDirectionalLight(Fragment frag, 
                               DirectionalLight light, 
                               out float4 ambient, 
                               out float4 diffuse, 
                               out float4 specular)
{
    ambient  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse  = float4(0.0f, 0.0f, 0.0f, 0.0f);
    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    ambient = frag.albedo * light.ambient;
    float diffuseFactor = dot(frag.normal.xyz, -1 * light.direction);
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        diffuse = diffuseFactor * (frag.albedo * light.diffuse);

        float3 view = reflect(light.direction, frag.normal.xyz);
        float specularFactor = pow(max(dot(view, frag.toEye.xyz), 0.0f), frag.specular.w);
        specular = specularFactor * float4(frag.specular.xyz, 0.0f) * light.specular;
    }
}

float4 main_PS(VertexPosNorColorOut pin) : SV_Target
{
    DirectionalLight light;
    light.ambient   = float4(.50f, .50f, .50f, 1.0f);
    light.diffuse   = float4(.40f, .40f, .40f, 1.0f);
    light.specular  = float4(.10f, .10f, .10f, 1.0f);
    light.direction = normalize(float3(-1.0f, -1.0f, -1.0f));

    Fragment frag;
    [flatten]
    if (albedo_map_bound)
        frag.albedo = g_albedo.Sample(g_sampler, pin.TexCord0);
    else
        frag.albedo = albedo_color;
    [flatten]
    if (specular_map_bound)
        frag.specular = g_specular.Sample(g_sampler, pin.TexCord0);
    else
        frag.specular = specular_color;
    [flatten]
    if (normal_map_bound)
        frag.normal = float4(g_normal.Sample(g_sampler, pin.TexCord0).xyz, 0.0f);
    else
        frag.normal = float4(pin.Normal, 0.0f);

    frag.toEye = float4(normalize(eye_pos.xyz - pin.PosL), .0f);

    float4 ambient  = float4(.0f, .0f, .0f, .0f);
    float4 diffuse  = float4(.0f, .0f, .0f, .0f);
    float4 specular = float4(.0f, .0f, .0f, .0f);

    float4 A, D, S;
    CalculateDirectionalLight(frag, light, A, D, S);
    ambient  += A;
    diffuse  += D;
    specular += S;

    float4 lit_color = ambient + diffuse + specular;
    lit_color.w = frag.albedo.w;

    return saturate(lit_color);
}
