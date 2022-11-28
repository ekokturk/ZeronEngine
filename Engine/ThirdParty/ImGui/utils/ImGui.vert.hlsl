//  Copyright (C) Eser Kokturk. All Rights Reserved.

// ImGui shader for the custom engine renderer
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
	location(2) float4 Color : COLOR;
};

struct VSOutput
{
	float4 Pos : SV_POSITION;
	location(0) float2 UV : TEXCOORD;
	location(1) float4 Color : COLOR;
};

VSOutput main(VSInput input)
{
	VSOutput output;
	output.UV = input.UV;
	output.Color = input.Color;
	output.Pos = mul(float4(input.Pos.xy, 0.f, 1.f), mvp.Projection);;
	return output;
}