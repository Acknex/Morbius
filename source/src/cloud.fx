float4x4 matWorldViewProj;
float4x4 matWorld;
float fAmbient;
float4 vecSunDir;

texture entSkin1;

sampler ColorMapSampler = sampler_state 
{ 
   Texture = <entSkin1>; 
   AddressU  = Clamp; 
   AddressV  = Clamp; 
}; 
    
void DiffuseVS( 
   in float4 InPos: POSITION, 
   in float3 InNormal: NORMAL, 
   in float2 InTex: TEXCOORD0, 
   out float4 OutPos: POSITION, 
   out float2 OutTex: TEXCOORD0, 
   out float3 OutNormal: TEXCOORD1) 
{ 
   OutPos = mul(InPos, matWorldViewProj); 
   OutNormal = normalize(mul(InNormal, matWorld));
   OutTex = InTex; 
} 
    
float4 DiffusePS( 
   in float2 InTex: TEXCOORD0, 
   in float3 InNormal: TEXCOORD1): COLOR 
{ 
InNormal = normalize(InNormal);

   float4 Color = tex2D(ColorMapSampler, InTex); 
   //float Diffuse = 0.75+0.5*saturate(dot(-vecSunDir, normalize(InNormal)))-saturate(-InNormal.y)*0.5;
   float Diffuse = saturate(0.75+0.4*saturate(dot(-vecSunDir, normalize(InNormal))));
   float4 final = (1+fAmbient)*Color*Diffuse;
   
   return final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
      VertexShader = compile vs_3_0 DiffuseVS(); 
      PixelShader  = compile ps_3_0 DiffusePS(); 
   } 
} 
