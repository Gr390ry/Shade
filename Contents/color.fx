//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProjection;
	float4 gWorldLightPosition;	
};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal: NORMAL;
	float2 mUV:TEXCOORD;
};

struct VS_OUTPUT
{
	float4 mPosition : SV_POSITION;
	float mDiffuse: TEXCOORD1;
	float2 mUV:TEXCOORD;
};

VS_OUTPUT VS(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
	float4 worldPosition = mul(Input.mPosition, gWorld);
	float3 worldNormal = mul(Input.mNormal, (float3x3)gWorld);
	float4 lightDir	= normalize(worldPosition - gWorldLightPosition);

	Output.mPosition = mul(worldPosition, gViewProjection);
	Output.mDiffuse = dot(-(float3)lightDir, normalize(worldNormal));
	Output.mUV = Input.mUV;

	return Output;
}

SamplerState TestSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

Texture2D gDiffuseMap;
float4 PS(VS_OUTPUT Input) : SV_Target
{
	float3 albedo = gDiffuseMap.Sample(TestSampler, Input.mUV);
	float diffuse = saturate(Input.mDiffuse * 0.5 + 0.5);

	//diffuse = ceil(diffuse * 5) / 5.0f;

	return float4(albedo * diffuse, 1);
}

technique11 ColorTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		//SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

