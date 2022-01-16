
Texture2D mDiffuseTexture	: TEXTURE : register(t0);
Texture2D mNormalTexture	: TEXTURE : register(t1);

SamplerState mDefaultSampler : SAMPLER : register(s0);

cbuffer lightBuffer : register(b0)
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

struct FS_INPUT
{
	float4 inPosition : SV_POSITION;
	float2 inTextureCoord: TEXTURE_COORD;
	float3 inNormal : NORMAL;
	float3 inWorldPos : WORLD_POSITION;
};


float4 main(FS_INPUT input) : SV_TARGET
{
	const float3 sampleColor = mDiffuseTexture.Sample(mDefaultSampler, input.inTextureCoord);
	const float3 ambientLight = ambientLightColor * ambientLightStrength;
	
	float3 appliedLight = ambientLight;
	
	const float3 vectorToLight = normalize(dynamicLightPosition - input.inWorldPos);
	// Ensure it is not negative
	float3 diffuseLightIntensity = max(dot(vectorToLight, input.inNormal), 0);

	const float distanceToLight = distance(dynamicLightPosition, input.inWorldPos);

	// Attenuation Factor = 1 / (a + b * x + c * x^2)
	const float attenuationFactor = 1 / (dynamicLightAttenuationA + dynamicLightAttenuationB * distanceToLight + dynamicLightAttenuationC * pow(distanceToLight, 2));

	diffuseLightIntensity *= attenuationFactor;
	
	const float3 diffuseLight = diffuseLightIntensity * dynamicLightStrength * dynamicLightColor;
	appliedLight += diffuseLight;
	
	float3 finalColor = sampleColor * appliedLight;
	return float4(finalColor, 1.f /*alpha*/);
}