//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProjection;
	float4	gWorldLightPosition;	
};

//SamplerState samAnisotropic
//{
//	Filter = ANISOTROPIC;
//	MaxAnisotropy = 4;
//
//	AddressU = WRAP;
//	AddressV = WRAP;
//};

struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 mNormal : NORMAL;
	float2 mUV : TEXCOORD;
	row_major float4x4 mWorld : WORLD;
	uint mInstanceId : SV_InstanceID;
};

struct VS_OUTPUT
{
	float4 mPosition : SV_POSITION;
	float mDiffuse: TEXCOORD1;
	float2 mUV:TEXCOORD;
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
	VS_OUTPUT Output;
	
	float4 worldPosition	= mul(Input.mPosition, Input.mWorld);
	float3 worldNormal		= mul(Input.mNormal, (float3x3)Input.mWorld);
	float4 lightDir			= normalize(worldPosition - gWorldLightPosition);

	Output.mPosition		= mul(worldPosition, gViewProjection);
	Output.mDiffuse			= dot(-(float3)lightDir, normalize(worldNormal));
	Output.mUV				= Input.mUV;

	return Output;
}

SamplerState TestSampler
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;
};

Texture2D gDiffuseMap;
float4 PS_MAIN(VS_OUTPUT Input) : SV_Target
{
	float3 albedo = (float3)gDiffuseMap.Sample(TestSampler, Input.mUV);
	float diffuse = saturate(Input.mDiffuse * 0.5 + 0.5);

	//diffuse = ceil(diffuse * 5) / 5.0f;

	return float4(albedo * diffuse, 1);
}

technique11 InstanceTechq
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_MAIN() ) );
		//SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_MAIN() ) );
    }
}

