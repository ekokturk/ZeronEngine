//cbuffer alphaBuffer : register(b0)
//{
//	float alpha;
//}

cbuffer lightBuffer : register(b0)
{
	float3 ambientLightColor;
	float ambientLightStrength = 1.f;
}

struct FS_INPUT
{
	float4 inPosition : SV_POSITION;
	float2 inTextureCoord: TEXTURE_COORD;
};

Texture2D tex : TEXTURE : register(t0);
SamplerState samplerState : SAMPLER : register(s0);

float4 main(FS_INPUT input) : SV_TARGET
{
	float3 pixelColor = tex.Sample(samplerState, input.inTextureCoord);
	float3 ambientLight = ambientLightColor * ambientLightStrength;
	float3 finalColor = pixelColor * ambientLight;
	return float4(finalColor, 1.f /*alpha*/);
}