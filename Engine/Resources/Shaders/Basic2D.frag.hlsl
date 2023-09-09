//  Copyright (C) Eser Kokturk. All Rights Reserved.

#define SHADER_FRAGMENT
#include "ZeronShaderUtils.h.hlsl"

struct Color {
	float4 color;
};

Texture2D mFontTexture	: TEXTURE : register(t1);
SamplerState mFontSampler : SAMPLER : register(s2);
cbuffer ColorBuffer : register(b3) {
	float4 color = float4(1.0f, 1.0f, 1.0f, 1.0f);
};

struct FSInput
{
	float4 Position : SV_POSITION;
	location(0) float2 UV: TEXCOORD;
};

float4 main(FSInput input) : SV_TARGET
{
	return color * mFontTexture.Sample(mFontSampler, input.UV);
}