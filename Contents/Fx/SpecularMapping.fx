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
// SpecularMapping
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string SpecularMapping_Pass_0_Model : ModelData = "D:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProjection : Projection;
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

struct VS_INPUT
{
   float4 mPosition:POSITION;
   float3 mNormal:NORMAL;
   float2 mUV:TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition:POSITION;
   float2 mUV:TEXCOORD0;
   float3 mDiffuse:TEXCOORD1;
   float3 mView:TEXCOORD2;
   float3 mReflection:TEXCOORD3;
};

VS_OUTPUT SpecularMapping_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   Output.mPosition = mul(Input.mPosition, matWorld);
   
   float3 lightDir = Output.mPosition.xyz - WorldLightPosition.xyz;
   lightDir = normalize(lightDir);
   float3 viewDir = Output.mPosition.xyz - WorldCameraPosition.xyz;
   normalize(viewDir);
   Output.mView = viewDir;
   
   Output.mPosition = mul(Output.mPosition, matView);
   Output.mPosition = mul(Output.mPosition, matProjection);
   
   
   float3 WorldNormal = mul(Input.mNormal, (float3x3)matWorld);
   WorldNormal = normalize(WorldNormal);
   
   Output.mDiffuse = dot(-lightDir, WorldNormal);
   Output.mReflection = reflect(lightDir, WorldNormal);
   Output.mUV = Input.mUV;
   
   return Output;
}
struct PS_INPUT
{
   float2 mUV:TEXCOORD0;
   float3 mDiffuse:TEXCOORD1;
   float3 mView:TEXCOORD2;
   float3 mReflection:TEXCOORD3;
};

texture DiffuseMap_Tex
<
   string ResourceName = "D:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Fieldstone.tga";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};
texture SpecularMap_Tex
<
   string ResourceName = "..\\Shader Example source\\05_DiffuseSpecularMapping\\fieldstone_SM.tga";
>;
sampler2D SpecularSampler = sampler_state
{
   Texture = (SpecularMap_Tex);
};
float3 LightColor
<
   string UIName = "LightColor";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
   float UIMin = -1.00;
   float UIMax = 1.00;
> = float3( 0.70, 0.70, 1.00 );

float4 SpecularMapping_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input):COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.mUV);   
  
   float3 diffuse = LightColor * albedo.rgb * saturate(Input.mDiffuse);
   
   float3 reflection = normalize(Input.mReflection);
   float3 viewDir = normalize(Input.mView);
   float3 specular = 0;
   
   if (diffuse.x > 0)
   {
      specular = saturate(dot(reflection, -viewDir));
      specular = pow(specular, 10.0f);
      
      float4 specularIntensity = tex2D(SpecularSampler, Input.mUV);
      specular *= specularIntensity.rgb * LightColor;
   }
   
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
   return float4(ambient + diffuse + specular, 1);
}
//--------------------------------------------------------------//
// Technique Section for SpecularMapping
//--------------------------------------------------------------//
technique SpecularMapping
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 SpecularMapping_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 SpecularMapping_Pass_0_Pixel_Shader_ps_main();
   }

}

