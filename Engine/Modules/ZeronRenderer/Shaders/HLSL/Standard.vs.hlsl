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
	float3 inInstancePos : INSTANCE_POS;
	uint inInstanceID : SV_InstanceID;
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
	float4 pos = float4(input.inPos, 1.0f);
	pos += float4(input.inInstancePos, 0.0f);
	output.outPosition = mul(pos, worldViewProjectionMatrix);
	output.outTextureCoord = input.inTextureCoord;
	output.outNormal = normalize(mul(float4(input.inNormal, 0.f), worldMatrix));
	output.outWorldPos = mul(pos, worldMatrix);
    return output;
}