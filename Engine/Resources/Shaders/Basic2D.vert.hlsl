//  Copyright (C) Eser Kokturk. All Rights Reserved.

#define SHADER_VERTEX
#include "ZeronShaderUtils.h.hlsl"

struct MVP {
	float4x4 Projection;
};

cbuffer mvpBuffer : register(b0) {
	MVP mvp;
};

struct VSInput
{
	location(0) float2 Pos : POSITION;
	location(1) float2 UV : TEXCOORD;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
	location(0) float2 UV : TEXCOORD;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.UV = input.UV;
	output.Pos = mul(float4(input.Pos.xy, 0.f, 1.f), mvp.Projection);;
	return output;
}