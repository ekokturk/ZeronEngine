//  Copyright (C) Eser Kokturk. All Rights Reserved.

#define SHADER_VERTEX
#include "ZeronShaderUtils.h.hlsl"

cbuffer cBuffer : register(b0)
{
	float4x4 worldViewProjectionMatrix;
	float4x4 worldMatrix;
};

struct VSInput
{
	location(0) float3 Pos : POSITION;
	location(1) float2 TextureCoord : TEXTURE_COORD;
	location(2) float3 Normal : NORMAL;
	location(3) float3 InstancePos : INSTANCE_POS;
};

struct VSOutput
{
	location(0) float4 Pos : SV_POSITION;
	location(1) float2 TextureCoord : TEXTURE_COORD;
	location(2) float3 Normal : NORMAL;
	location(3) float3 WorldPos : WORLD_POSITION;
};

VSOutput main(VSInput input, uint inInstanceId : SV_InstanceID)
{
	VSOutput output;
	float4 pos = float4(input.Pos, 1.0f);
	pos += float4(input.InstancePos, 0.0f);
	output.Pos = mul(pos, worldViewProjectionMatrix);
	output.TextureCoord = input.TextureCoord;
	output.Normal = normalize(mul(float4(input.Normal, 0.f), worldMatrix));
	output.WorldPos = mul(pos, worldMatrix);
    return output;
}