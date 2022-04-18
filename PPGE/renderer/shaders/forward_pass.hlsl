struct VertexPosNorColorIn
{
    float3 PosL   : POSITION;
    uint   Color  : COLOR;
};

struct VertexPosNorColorOut
{
    float4 PosH   : SV_POSITION;
    float3 PosL   : POSITION;
    uint   Color  : COLOR;
};

cbuffer g_PerFrame  : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewProj;
    float3   cameraPosition;
    float    padding;
};

cbuffer g_PerObject : register(b1)
{
	float4x4 model;
    float4x4 modelInvTran;
};

cbuffer g_Color  : register(b2)
{
    float4 diffuse;
    float4 specular;
    bool is_textureless_diffuse;
    bool is_textureless_speculer;
};

Texture2D g_diffuse  : register(t0);
Texture2D g_specular : register(t1);
Texture2D g_normal   : register(t2);

SamplerState g_sampler : register(s0);

VertexPosNorColorOut main_VS(VertexPosNorColorIn vin)
{
	VertexPosNorColorOut vout;

    // Homogenous device position
    vout.PosH    = mul(float4(vin.PosL, 1.0), viewProj);
    
    // World position
    vout.PosL = mul(float4(vin.PosL, 1.0), model).xyz;

    // Vertex color
	vout.Color = vin.Color; 
    
    return vout;
}

float4 main_PS(VertexPosNorColorOut pin) : SV_Target
{
    float r = pin.Color & 0xff000000;
    float g = pin.Color & 0x00ff0000;
    float b = pin.Color & 0x0000ff00;
    float a = pin.Color & 0x000000ff;
    return float4(1, 1, 1, 1);
}
