//**************************************************************//
//  Effect File exported by RenderMonkey 1.6
//
//  - Although many improvements were made to RenderMonkey FX  
//    file export, there are still situations that may cause   
//    compilation problems once the file is exported, such as  
//    occasional naming conflicts for methods, since FX format 
//    does not support any notions of name spaces. You need to 
//    try to create workspaces in such a way as to minimize    
//    potential naming conflicts on export.                    
//    
//  - Note that to minimize resulting name collisions in the FX 
//    file, RenderMonkey will mangle names for passes, shaders  
//    and function names as necessary to reduce name conflicts. 
//**************************************************************//

//--------------------------------------------------------------//
// NormalMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string NormalMapping_Pass_0_Model : ModelData = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";


struct VS_INPUT
{
   float4 mPosition:POSITION;
   float3 mNormal:NORMAL;
   float3 mTangent:TANGENT;
   float3 mBinormal:BINORMAL;
   float2 mUV:TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition:POSITION;
   float2 mUV:TEXCOORD0;
   float3 mLightDir:TEXCOORD1;
   float3 mViewDir:TEXCOORD2;
   float3 mTangent:TEXCOORD3;
   float3 mBinormal:TEXCOORD4;
   float3 mNormal:TEXCOORD5;
};

float4x4 WorldMatrix : World;
float4x4 WorldViewProjectionMatrix : WorldViewProjection;
float4 WorldLightPosition
<
   string UIName = "WorldLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 500.00, 500.00, -500.00, 1.00 );
float4 WorldCameraPosition : ViewPosition;

VS_OUTPUT NormalMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition   = mul(Input.mPosition,WorldViewProjectionMatrix);
   Output.mUV         = Input.mUV;
   
   
   float4 worldPosition = mul(Input.mPosition, WorldMatrix);
   float3 lightDir    = normalize(worldPosition.xyz - WorldLightPosition.xyz);
   float3 viewDir     = normalize(worldPosition.xyz - WorldCameraPosition.xyz);
   
   Output.mLightDir   = lightDir;
   Output.mViewDir    = viewDir;   
   
   float3 worldNormal = mul(Input.mNormal, (float3x3)WorldMatrix);
   Output.mNormal     = normalize(worldNormal);
   float3 worldTangent= mul(Input.mTangent, (float3x3)WorldMatrix);
   Output.mTangent    = normalize(worldTangent);
   float3 worldBinormal= mul(Input.mBinormal, (float3x3)WorldMatrix);
   Output.mBinormal   = normalize(worldBinormal);
      
   return Output;
}
struct PS_INPUT
{
   float2 mUV:TEXCOORD0;
   float3 mLightDir:TEXCOORD1;
   float3 mViewDir:TEXCOORD2;
   float3 mTangent:TEXCOORD3;
   float3 mBinormal:TEXCOORD4;
   float3 mNormal:TEXCOORD5;
};

texture DiffuseMap_Tex
<
   string ResourceName = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Fieldstone.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
texture SpecularMap_Tex
<
   string ResourceName = "..\\C++\\source\\07_NormalMapping\\fieldstone_SM.tga";
>;
sampler2D SpecularSampler = sampler_state
{
   Texture = (SpecularMap_Tex);
};
texture NormalMap_Tex
<
   string ResourceName = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\FieldstoneBumpDOT3.tga";
>;
sampler2D NormalSampler = sampler_state
{
   Texture = (NormalMap_Tex);
};
float3 LightColor
<
   string UIName = "LightColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 1.00, 1.00, 1.00 );

float4 NormalMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input):COLOR
{
   float3 tangentNormal = tex2D(NormalSampler, Input.mUV);
   tangentNormal = normalize(tangentNormal * 2 - 1);
      
   float3x3 TBN = float3x3(normalize(Input.mTangent), 
                           normalize(Input.mBinormal), 
                           normalize(Input.mNormal));
   
   TBN = transpose(TBN);
   
   float3 worldNormal = mul(TBN, tangentNormal);
   float3 lightDir = normalize(Input.mLightDir);
   float3 diffuse = saturate(dot(worldNormal, -lightDir));
         
   float3 albedo = tex2D(DiffuseSampler, Input.mUV);
   diffuse = LightColor * albedo * diffuse;
      
   float3 specular = 0;
   if (diffuse.x > 0)
   {
      float3 Reflection = reflect(lightDir, worldNormal);
      float3 viewDir = normalize(Input.mViewDir);
      specular = saturate(dot(Reflection, -viewDir));
      specular = pow(specular, 10.0f);
      
      float3 specularIntensity = tex2D(SpecularSampler, Input.mUV);
      specular *= specularIntensity.rgb * LightColor;
   }
      
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   
   return float4(ambient + diffuse + specular, 1);
}
//--------------------------------------------------------------//
// Technique Section for NormalMapping
//--------------------------------------------------------------//
technique NormalMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 NormalMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 NormalMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

