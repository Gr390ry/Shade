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

VS_OUTPUT Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main(VS_INPUT input) : SV_POSITION
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
      VertexShader = compile vs_5_0 Effect_Group_1_Effect1_Pass_0_Vertex_Shader_vs_main();
      PixelShader = compile ps_5_0 Effect_Group_1_Effect1_Pass_0_Pixel_Shader_ps_main();
   }
}

