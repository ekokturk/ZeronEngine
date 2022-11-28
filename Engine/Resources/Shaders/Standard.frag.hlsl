//  Copyright (C) Eser Kokturk. All Rights Reserved.

#define SHADER_FRAGMENT
#include "ZeronShaderUtils.h.hlsl"

Texture2D mDiffuseTexture	: TEXTURE : register(t1);
Texture2D mNormalTexture	: TEXTURE : register(t2);

SamplerState mDefaultSampler : SAMPLER : register(s3);

cbuffer lightBuffer : register(b4)
{
	float3 ambientLightColor;
	float ambientLightStrength;

	float3 dynamicLightColor;
	float dynamicLightStrength;
	float3 dynamicLightPosition;

	float dynamicLightAttenuationA;
	float dynamicLightAttenuationB;
	float dynamicLightAttenuationC;
}

struct FSInput
{
	location(0) float4 Position : SV_POSITION;
	location(1) float2 TextureCoord: TEXTURE_COORD;
	location(2) float3 Normal : NORMAL;
	location(3) float3 WorldPos : WORLD_POSITION;
};


float4 main(FSInput input) : SV_TARGET
{
	const float3 sampleColor = mDiffuseTexture.Sample(mDefaultSampler, input.TextureCoord);
	const float3 ambientLight = ambientLightColor * ambientLightStrength;
	
	float3 appliedLight = ambientLight;
	
	const float3 vectorToLight = normalize(dynamicLightPosition - input.WorldPos);
	// Ensure it is not negative
	float3 diffuseLightIntensity = max(dot(vectorToLight, input.Normal), 0);

	const float distanceToLight = distance(dynamicLightPosition, input.WorldPos);

	// Attenuation Factor = 1 / (a + b * x + c * x^2)
	const float attenuationFactor = 1 / (dynamicLightAttenuationA + dynamicLightAttenuationB * distanceToLight + dynamicLightAttenuationC * pow(distanceToLight, 2));

	diffuseLightIntensity *= attenuationFactor;
	
	const float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
	appliedLight += diffuseLight;
	
	float3 finalColor = sampleColor * appliedLight;
	return float4(finalColor, 1.f /*alpha*/);
}