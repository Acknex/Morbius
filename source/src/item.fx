float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecAmbient;
float4 vecViewDir;
float4 vecSunDir;
float4 vecTime;
float4 vecSkill41;

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
   out float3 OutTex: TEXCOORD0, 
   out float3 OutNormal: TEXCOORD1) 
{ 
   OutPos = mul(InPos, matWorldViewProj); 
   OutNormal = normalize(mul(InNormal, matWorld));
   OutTex = float3(InTex,InPos.y);
} 
    
float4 DiffusePS( 
   in float3 InTex: TEXCOORD0, 
   in float3 InNormal: TEXCOORD1): COLOR 
{ 
InNormal = normalize(InNormal);
   float4 Color = tex2D(ColorMapSampler, InTex); 
   float Diffuse = 0.5+0.5*saturate(dot(-vecSunDir, InNormal)*2); 
   float highlight = saturate((sin(vecTime.w*0.2+InTex.z*vecSkill41.x)-0.8));
   float4 final = Color*Diffuse+highlight*highlight*4;

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
