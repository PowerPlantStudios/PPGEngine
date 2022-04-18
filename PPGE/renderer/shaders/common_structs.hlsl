struct VertexPosColorIn
{
    float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexPosColorOut
{
    float4 PosH  : SV_POSITION;
    float3 PosL  : POSITION;
    float4 Color : COLOR;
};

struct VertexPosNorColorIn
{
    float3 PosL   : POSITION;
    float3 Normal : NORMAL;
    float4 Color  : COLOR;
};

struct VertexPosNorColorOut
{
    float4 PosH   : SV_POSITION;
    float3 PosL   : POSITION;
    float3 Normal : NORMAL;
    float4 Color  : COLOR;
};

struct VertexFullIn
{
	float3 PosL       : POSITION;
    float3 Normal     : NORMAL;
    float3 Tangent    : TANGENT;
    float3 Bitangent  : BITANGENT;
    float3 Color      : COLOR;
    float2 Texture0   : TEXTURE0;
    float2 Texture1   : TEXTURE1;
    float2 Texture2   : TEXTURE2;
};

struct VertexFullOut
{
	float4 PosH      : SV_POSITION;
	float3 PosL      : POSITION;
    float3 Normal    : NORMAL;
    float3 Tangent   : TANGENT;
    float3 Bitangent : BITANGENT;
    float3 Color     : COLOR;
    float2 Texture0  : TEXTURE0;
    float2 Texture1  : TEXTURE1;
    float2 Texture2  : TEXTURE2;
};