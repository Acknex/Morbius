float level_change_transition_progress_var;
float4 vecViewPort;

texture TargetMap;

sampler TargetMapSampler = sampler_state 
{ 
   Texture = <TargetMap>; 
   AddressU  = Clamp; 
   AddressV  = Clamp; 
}; 
    
float4 Transition(in float2 InTex: TEXCOORD0): COLOR 
{ 
	//float factor = (1-level_change_transition_progress_var*0.01)*vecViewPort.xy*0.25;
   //InTex = round(InTex.xy*factor)/factor;
	float factor = 100-level_change_transition_progress_var;
   InTex = lerp(InTex,round(InTex.xy*factor)/factor,level_change_transition_progress_var*0.01);
   float4 Color = tex2D(TargetMapSampler, InTex);
   float4 final = Color*(1-level_change_transition_progress_var*0.01);
   
   return final;
} 
 
technique DiffuseTechnique 
{ 
   pass P0 
   { 
      PixelShader  = compile ps_2_0 Transition(); 
   } 
} 
