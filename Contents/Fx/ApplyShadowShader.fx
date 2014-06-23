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
// ApplyShadow
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// ApplyShadowTorus
//--------------------------------------------------------------//
string ApplyShadow_ApplyShadowTorus_Torus : ModelData = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Torus.3ds";

struct VS_INPUT
{
   float4 mPosition:POSITION;
   float3 mNormal:NORMAL;
};

struct VS_OUTPUT
{
   float4 mPosition:POSITION;
   float4 mClipPosition:TEXCOORD1;
   float mDiffuse:TEXCOORD2;
};

float4x4 WorldMatrix : World;
float4x4 LightViewMatrix
<
   string UIName = "LightViewMatrix";
   string UIWidget = "Numeric";
   bool UIVisible =  false;
> = float4x4( 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00, 0.00, 0.00, 0.00, 0.00, 1.00 );
float4x4 LightProjectionMatrix : Projection;
float4x4 ViewProjectionMatrix : ViewProjection;
float4 WorldLightPosition
<
   string UIName = "WorldLightPosition";
   string UIWidget = "Direction";
   bool UIVisible =  false;
   float4 UIMin = float4( -10.00, -10.00, -10.00, -10.00 );
   float4 UIMax = float4( 10.00, 10.00, 10.00, 10.00 );
   bool Normalize =  false;
> = float4( 50.00, 500.00, -500.00, 1.00 );

VS_OUTPUT ApplyShadow_ApplyShadowTorus_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   float4x4 lightViewMatrix = LightViewMatrix;
   
   float4 worldPosition = mul(Input.mPosition, WorldMatrix);
   
   Output.mPosition = mul(worldPosition, ViewProjectionMatrix);   
   
   Output.mClipPosition = mul(worldPosition, LightViewMatrix);
   Output.mClipPosition = mul(Output.mClipPosition, LightProjectionMatrix);
   
   float3 worldNormal = normalize(mul(Input.mNormal, (float3x3)WorldMatrix));
   float3 lightDir = normalize(worldPosition.xyz - WorldLightPosition.xyz);   
   Output.mDiffuse = dot(-lightDir, worldNormal);
   
   return Output;
}



struct PS_INPUT
{
   float4 mClipPosition:TEXCOORD1;
   float mDiffuse:TEXCOORD2;
};

texture ShadowMap_Tex
<
   string ResourceName = ".\\";
>;
sampler2D ShadowSampler = sampler_state
{
   Texture = (ShadowMap_Tex);
};

float4 ApplyShadow_ApplyShadowTorus_Pixel_Shader_ps_main(PS_INPUT Input):COLOR
{
   float depth = Input.mClipPosition.z / Input.mClipPosition.w;
   float3 diffuse = saturate(Input.mDiffuse * 0.5f + 0.5f);
   
   float2 uv = Input.mClipPosition.xy / Input.mClipPosition.w;   
   uv.y = -uv.y;
   uv = uv * 0.5f + 0.5f;
   
   float shadowDepth = tex2D(ShadowSampler, uv).r;
   
   if (depth > shadowDepth + 0.0000125f)
   {
      diffuse *= 0.5f;
   }

   return float4(diffuse, 1);
}
//--------------------------------------------------------------//
// Technique Section for ApplyShadow
//--------------------------------------------------------------//
technique ApplyShadow
{
   pass ApplyShadowTorus
   {
      VertexShader = compile vs_2_0 ApplyShadow_ApplyShadowTorus_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ApplyShadow_ApplyShadowTorus_Pixel_Shader_ps_main();
   }

}

