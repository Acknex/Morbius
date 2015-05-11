float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecSunDir;
float4 vecViewDir;
float fAmbient;

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
   float Diffuse = 0.5+0.65*saturate(dot(-vecSunDir, InNormal)*6); 
   //float highlight = 1-saturate(dot(-vecViewDir, InNormal));
   float4 final = Color*(Diffuse); //+highlight*0
   
   return (1+fAmbient)*final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
      VertexShader = compile vs_2_0 DiffuseVS(); 
      PixelShader  = compile ps_2_0 DiffusePS(); 
   } 
} 
