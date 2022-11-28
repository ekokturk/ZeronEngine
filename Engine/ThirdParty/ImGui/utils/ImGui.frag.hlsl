//  Copyright (C) Eser Kokturk. All Rights Reserved.

// ImGui shader for the custom engine renderer
#define SHADER_FRAGMENT
#include "ZeronShaderUtils.h.hlsl"

Texture2D mFontTexture	: TEXTURE : register(t1);
SamplerState mFontSampler : SAMPLER : register(s2);

struct FSInput
{
	float4 Position : SV_POSITION;
	location(0) float2 UV: TEXCOORD;
	location(1) float4 Color : COLOR;
};

float4 main(FSInput input) : SV_TARGET
{
	return input.Color * mFontTexture.Sample(mFontSampler, input.UV);
}