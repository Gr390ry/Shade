//***************************************************************************************
// color.fx by Frank Luna (C) 2011 All Rights Reserved.
//
// Transforms and colors geometry.
//***************************************************************************************

cbuffer cbPerObject
{
	float4x4 gWorld;
	float4x4 gViewProjection;
	//float4 gWorldLightPosition;	
};

struct VS_INPUT
{
	float4 _position	: POSITION;
	float4 _color		: COLOR;
	//float3 mNormal		: NORMAL;
	//float2 mUV			: TEXCOORD;
};

struct VS_OUTPUT
{
	float4 _position	: SV_POSITION;
	float4 _color		: COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
	VS_OUTPUT output;
	
	//float4 worldPosition = mul(Input.mPosition, gWorld);
	//float3 worldNormal	= mul(Input.mNormal, (float3x3)gWorld);
	//float4 lightDir		= normalize(worldPosition - gWorldLightPosition);

	//Output.mPosition	= mul(worldPosition, gViewProjection);
	//Output.mDiffuse		= dot(-(float3)lightDir, normalize(worldNormal));
	//Output.mUV			= Input.mUV;

	output._position		= mul(input._position, gWorld);
	output._position		= mul(output._position, gViewProjection );
	output._color			= input._color;
	return output;
}

//SamplerState TestSampler
//{
//	Filter = ANISOTROPIC;
//	MaxAnisotropy = 4;
//};
//
//Texture2D gDiffuseMap;

float4 PS(VS_OUTPUT input) : SV_Target
{
	return input._color;

	//float3 albedo = gDiffuseMap.Sample(TestSampler, Input.mUV);
	//float diffuse = saturate(Input.mDiffuse * 0.5 + 0.5);

	//return float4(albedo * diffuse, 1);
}

technique11 BasicTech
{
    pass P0
    {
        SetVertexShader( CompileShader( vs_5_0, VS() ) );
		SetGeometryShader( NULL );
        SetPixelShader( CompileShader( ps_5_0, PS() ) );
    }
}

