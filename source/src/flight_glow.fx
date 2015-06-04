float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecViewDir;

void DiffuseVS( 
in float4 InPos: POSITION, 
in float3 InNormal: NORMAL, 
in float2 InTex: TEXCOORD0, 
out float4 OutPos: POSITION, 
out float2 OutTex: TEXCOORD0, 
out float3 OutNormal: TEXCOORD1,
uniform float scale) 
{ 
InPos.yz += 0.1*(InPos.x-0)*scale*InNormal.yz;
InPos.x += saturate(InPos.x-30)*(2-scale)*20;

	OutPos = mul(InPos, matWorldViewProj); 
	OutNormal = normalize(mul(InNormal, matWorld));
	OutTex = InTex; 
} 

float4 DiffusePS( 
in float2 InTex: TEXCOORD0, 
in float3 InNormal: TEXCOORD1): COLOR 
{ 
InNormal = normalize(InNormal);
	float4 Color = float4(1,0.85,0.6,1); 
	float diffuse = saturate(dot(-vecViewDir,InNormal));
	float4 final = Color;
	final.a = (1-InTex.x)*0.25*diffuse*diffuse;
	
	return final;
} 

technique DiffuseTechnique 
{ 
	pass P0 
	{ 
		zwriteEnable = false;
		alphaBlendEnable = true;
		cullmode = none;
		destBlend = one;
		VertexShader = compile vs_2_0 DiffuseVS(0); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
	pass P1
	{ 
		VertexShader = compile vs_2_0 DiffuseVS(1); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
	pass P2 
	{ 
		VertexShader = compile vs_2_0 DiffuseVS(1.5); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
	pass P3 
	{ 
		VertexShader = compile vs_2_0 DiffuseVS(2); 
		PixelShader  = compile ps_2_0 DiffusePS(); 
	} 
} 
