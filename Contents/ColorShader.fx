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
// Effect Group 1
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Effect1
//--------------------------------------------------------------//
//--------------------------------------------------------------//
// Pass 0
//--------------------------------------------------------------//
string Effect_Group_1_Effect1_Pass_0_Model : ModelData = "..\\Program Files (x86)\\AMD\\RenderMonkey 1.82\\Examples\\Media\\Models\\Sphere.3ds";

struct VS_OUTPUT
{ 
   float4 mPosition:POSITION;
};

struct VS_INPUT
{
   float4 mPosition:POSITION;
};

float4x4 WorldMatrix : World;
float4x4 ViewMatrix : View;
float4x4 ProjectionMatrix : Projection;

VS_OUTPUT Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main(VS_INPUT input)
{
   VS_OUTPUT output;
   output.mPosition = mul(input.mPosition, WorldMatrix);
   output.mPosition = mul(output.mPosition, ViewMatrix);
   output.mPosition = mul(output.mPosition, ProjectionMatrix);
   return output;
}
float4 Effect_Group_1_Effect1_Pass_0_Pixel_Shader_ps_main() : COLOR0
{
   return float4(1.0f, 0.0f, 0.0f, 1.0f);
}
//--------------------------------------------------------------//
// Technique Section for Effect Group 1
//--------------------------------------------------------------//
technique Effect1
{
   pass Pass_0
   {
      VertexShader = compile vs_2_0 Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_2_0 Effect_Group_1_Effect1_Pass_0_Pixel_Shader_ps_main();
   }

}

