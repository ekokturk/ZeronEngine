#pragma pack_matrix(row_major)

cbuffer cBuffer : register(b0)
{
	float4x4 worldViewProjectionMatrix;
	float4x4 worldMatrix;
};

struct VS_INPUT
{
	float3 inPos : POSITION;
	float2 inTextureCoord : TEXTURE_COORD;
	float3 inNormal : NORMAL;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float2 outTextureCoord : TEXTURE_COORD;
	float3 outNormal : NORMAL;
	float3 outWorldPos : WORLD_POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.outPosition = mul(float4(input.inPos, 1.f), worldViewProjectionMatrix);
	output.outTextureCoord = input.inTextureCoord;
	output.outNormal = normalize(mul(float4(input.inNormal, 0.f), worldMatrix));
	output.outWorldPos = mul(float4(input.inPos, 1.f), worldMatrix);
    return output;
}