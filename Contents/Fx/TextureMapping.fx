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
// Default_DirectX_Effect
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Default_DirectX_Effect_Pass_0_Model : ModelData = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";


struct VS_INPUT
{
   float4 mPosition:POSITION;
   float2 mTextureCoord:TEXCOORD0;
};

struct VS_OUTPUT
{
   float4 mPosition:POSITION;
   float2 mTextureCoord:TEXCOORD0;
};

float4x4 matWorld : World;
float4x4 matView : View;
float4x4 matProjection : Projection;

VS_OUTPUT Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main(VS_INPUT Input)
{
   VS_OUTPUT Output;
   Output.mPosition = mul(Input.mPosition, matWorld);
   Output.mPosition = mul(Output.mPosition, matView);
   Output.mPosition = mul(Output.mPosition, matProjection);
   Output.mTextureCoord = Input.mTextureCoord;
   
   return Output;
}
struct PS_INPUT
{
   float2 TextureCoord:TEXCOORD0;
};

texture DiffuseMap_Tex
<
   string ResourceName = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Textures\\Earth.jpg";
>;
sampler2D DiffuseSampler = sampler_state
{
   Texture = (DiffuseMap_Tex);
};

float4 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main(PS_INPUT Input):COLOR
{
   float4 albedo = tex2D(DiffuseSampler, Input.TextureCoord);
   return albedo.rgba;
}
//--------------------------------------------------------------//
// Technique Section for Default_DirectX_Effect
//--------------------------------------------------------------//
technique Default_DirectX_Effect
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Default_DirectX_Effect_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Default_DirectX_Effect_Pass_0_Pixel_Shader_ps_main();
   }

}

