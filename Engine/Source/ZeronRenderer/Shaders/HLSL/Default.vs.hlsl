cbuffer cBuffer : register(b0)
{
	float4x4 mat;
};

struct VS_INPUT
{
	float3 inPos : POSITION;
	float2 inTextureCoord : TEXTURE_COORD;
};

struct VS_OUTPUT
{
	float4 outPosition : SV_POSITION;
	float2 outTextureCoord : TEXTURE_COORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.outPosition = mul(float4(input.inPos, 1.f), mat);
	output.outTextureCoord = input.inTextureCoord;
    return output;
}