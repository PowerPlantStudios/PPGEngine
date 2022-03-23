cbuffer g_PerFrame  : register(b0)
{
    float4x4 view;
    float4x4 proj;
    float4x4 viewProj;
    float3  cameraPosition;
    float   padding;
};

cbuffer g_PerObject : register(b1)
{
	float4x4 model;
    float4x4 modelInvTran;
};

struct VertexIn
{
	float3 PosL    : POSITION;
    float3 Normal  : NORMAL; 
    float2 Texture : TEXTURE;
};

struct VertexOut
{
	float4 PosH    : SV_POSITION;
	float3 PosL    : POSITION;
    float3 Normal  : NORMAL; 
    float2 Texture : TEXTURE;
    float3 EyePos  : VECTOR0;
};

VertexOut main(VertexIn vin)
{
	VertexOut vout;

    // Homogenous device position
    vout.PosH    = mul(float4(vin.PosL, 1.0), viewProj);
    
    // World position
    vout.PosL = mul(float4(vin.PosL, 1.0), model).xyz;

    // World normals    
    vout.Normal = mul(vin.Normal, model).xyz;
    vout.Normal = normalize(vout.Normal);

	vout.Texture.x = vin.Texture.x; 
	vout.Texture.y = 1 - vin.Texture.y;

    vout.EyePos = cameraPosition.xyz;

    return vout;
}
