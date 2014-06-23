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
// ColorConversion
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string ColorConversion_Pass_0_Model : ModelData = "D:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\ninjaHead.obj";

texture Scene_Tex : RenderColorTarget
<
   float2 ViewportRatio={1.0,1.0};
   string Format="D3DFMT_A8R8G8B8";
   float  ClearDepth=1.000000;
   int    ClearColor=-1;
>;

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
   float3 mViewDir:TEXCOORD2;
   float3 mReflection:TEXCOORD3;
};

float4x4 WorldMatrix : World;
float4x4 ViewProjectionMatrix : ViewProjection;
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

VS_OUTPUT ColorConversion_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   
   float4 worldPosition = mul(Input.mPosition, WorldMatrix);   
   float3 worldNormal = normalize(mul(Input.mNormal, (float3x3)WorldMatrix));
   
   
   float3 lightDir = normalize(worldPosition.xyz - WorldLightPosition.xyz);
   float3 viewDir = normalize(worldPosition.xyz - WorldCameraPosition.xyz);
         
   Output.mPosition = mul(worldPosition, ViewProjectionMatrix);
   Output.mDiffuse = dot(-lightDir, worldNormal);
   Output.mViewDir = viewDir;
   Output.mReflection = reflect(lightDir, worldNormal);
   Output.mUV = Input.mUV;
   
   return Output;
}





struct PS_INPUT
{
   float3 mUV:TEXCOORD0;
   float3 mDiffuse:TEXCOORD1;
   float3 mViewDir:TEXCOORD2;
   float3 mReflection:TEXCOORD3;
};

texture DiffuseMap_Tex
<
   string ResourceName = "D:\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

float4 ColorConversion_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input):COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.mUV);
   float3 diffuse = saturate(Input.mDiffuse * 0.5 + 0.5) * albedo;
   float3 reflection = normalize(Input.mReflection);
   float3 viewDir = normalize(Input.mViewDir);
   float3 specular = 0;
   
   if (diffuse.x > 0)
   {
      specular = saturate(dot(reflection, -viewDir));
      specular = pow(specular, 20.0f);
   }
   
   float3 ambient = float3(0.1f, 0.1f, 0.1f) * albedo;
     
   return float4(ambient + diffuse + specular, 1);
}

//--------------------------------------------------------------//
// Technique Section for ColorConversion
//--------------------------------------------------------------//
technique ColorConversion
{
   pass Pass_0
   <
      string Script = "RenderColorTarget0 = Scene_Tex;"
                      "ClearColor = (255, 255, 255, 255);"
                      "ClearDepth = 1.000000;";
   >
   {
      VertexShader = compile vs_2_0 ColorConversion_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 ColorConversion_Pass_0_Pixel_Shader_ps_main();
   }

}

