float4x4 matWorldViewProj;
float4x4 matWorld;
float fAmbient;
float4 vecSunDir;
float4 vecTime;

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
out float3 OutNormal: TEXCOORD1,
out float3 OutPos2: TEXCOORD2) 
{ 
	OutPos = mul(InPos, matWorldViewProj); 
	OutNormal = mul(InNormal, matWorld).xyz;
	OutTex = InTex;
	OutPos2 = mul(InPos, matWorld).xyz;
} 

float4 DiffusePS( 
in float2 InTex: TEXCOORD0, 
in float3 InNormal: TEXCOORD1,
in float3 InPos: TEXCOORD2): COLOR 
{ 
	InNormal = normalize(InNormal);
	float4 Color = tex2D(ColorMapSampler, InTex); 
	float Diffuse = 0.5+0.5*saturate(dot(-vecSunDir, InNormal)*2); 
	float3 hlDir = float3(-3,-4,-0);
	float3 hlDirN = normalize(hlDir);
	float dotvalue = dot(InPos,hlDirN)/16.0;
	float highlight = sin(dotvalue-vecTime.w*0.125)*0.5+0.5;
	highlight = saturate((highlight*highlight-0.33))*0.85;
	float4 final = (1+fAmbient)*Color*Diffuse+highlight*highlight;

	return final;
} 

technique DiffuseTechnique 
{ 
	pass P0 
	{ 
		VertexShader = compile vs_2_0 DiffuseVS(); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
} 
