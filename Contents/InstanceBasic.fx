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

SamplerState samAnisotropic
{
	Filter = ANISOTROPIC;
	MaxAnisotropy = 4;

	AddressU = WRAP;
	AddressV = WRAP;
};

struct VS_INPUT
{
	float3 mPosition	: POSITION;
	float3 mNormal		: NORMAL;
	float2 mUV			: TEXCOORD0;
	row_major float4x4 mWorld : WORLD;
	float4 mColor		: COLOR;
	uint mInstanceId	: SV_InstanceID;	
};

struct VS_OUTPUT
{
	float3	_worldPosition			: POSITION;
	float4	_hPosition				: SV_POSITION;
	float3	_worldNormal			: NORMAL;
	float2	_textureUV				: TEXCOORD0;
	float	_diffuse				: TEXCOORD1;	
};

VS_OUTPUT VS_MAIN(VS_INPUT Input)
{
	VS_OUTPUT output;
	
	//float4 worldPosition	= mul(Input.mPosition, Input.mWorld);
	//float3 worldNormal		= mul(Input.mNormal, (float3x3)Input.mWorld);
	//float4 lightDir			= normalize(worldPosition - gWorldLightPosition);

	//세계 공간으로 변환
	output._worldPosition	= mul(float4(Input.mPosition, 1), Input.mWorld).xyz;
	//output._worldNormal		= mul(Input.mNormal, (float3x3)Input.mWorld);
	//동차 공간으로 변환
	output._hPosition		= mul(float4(output._worldPosition, 1), gViewProjection);

	//텍스쳐 uv값
	//output._textureUV		= Input.mUV;

	/*float4 lightDir			= normalize(output._worldPosition - gWorldLightPosition);
	output._diffuse			= dot(-(float3)lightDir, normalize(output._worldNormal));
	*/

	/*output.mPosition		= mul(worldPosition, gViewProjection);
	output.mDiffuse			= dot(-(float3)lightDir, normalize(worldNormal));
	output.mUV				= Input.mUV;*/

	return output;
}

//Texture2D gDiffuseMap;
float4 PS_MAIN(VS_OUTPUT Input) : SV_Target
{
	//float3 albedo = (float3)gDiffuseMap.Sample(TestSampler, Input.mUV);
	//float diffuse = saturate(Input._diffuse * 0.5 + 0.5);

	//diffuse = ceil(diffuse * 5) / 5.0f;

	//return float4(albedo * diffuse, 1);


	return float4(1, 1, 1, 1);
}

technique11 InstanceTechq
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS_MAIN() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS_MAIN() ) );
    }
}

