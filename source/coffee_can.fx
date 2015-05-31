const float4x4 matWorldViewProj; // World*view*projection matrix. 
const float4x4 matView; // World*view*projection matrix. 
const float4x4 matWorld; // World matrix. 
const float4 vecViewDir;  // The sun direction vector.
//float sk_highlight_var;
float4 vecSkill41;
float fAmbient;
//bool PASS_SOLID;

texture entSkin1; // Model texture
texture bmp_glocke_chrome_bmap;

sampler ColorMapSampler = sampler_state 
{ 
	Texture = <entSkin1>; 
	AddressU  = Wrap; 
	AddressV  = Wrap; 
}; 

sampler ChromeMapSampler = sampler_state 
{ 
	Texture = <bmp_glocke_chrome_bmap>; 
	AddressU  = Wrap; 
	AddressV  = Wrap; 
}; 

void DiffuseVS( 
in float4 InPos: POSITION, 
in float3 InNormal: NORMAL, 
in float2 InTex: TEXCOORD0, 
out float4 OutPos: POSITION, 
out float2 OutTex: TEXCOORD0, 
out float3 OutNormal: TEXCOORD1,
out float3 OutLookup: TEXCOORD2) 
{ 
	OutPos = mul(InPos, matWorldViewProj); 
	OutNormal = normalize(mul(InNormal, matWorld));
	OutTex = InTex;
	OutLookup = mul(OutNormal,matView)*0.45+0.5; //*float3(1,0.25,1)
} 

// Pixel Shader: 
float4 DiffusePS( 
in float2 InTex: TEXCOORD0, 
in float3 InNormal: TEXCOORD1,
in float2 Lookup: TEXCOORD2): COLOR 
{ 
InNormal = normalize(InNormal);

	float chrome = tex2D(ChromeMapSampler,Lookup.xy+InTex.xy*0.25*(1+vecSkill41.z)); //*float4(0.5,0.1,0.1,1)
	float4 Color = tex2D(ColorMapSampler, InTex);
	float diffuse = 1-length(InNormal.xy)*0.85; //1+InNormal.z; //
	float diffuse2 = 1+InNormal.z;

	float4 final = (diffuse+chrome*0.45)*Color+chrome*0.85+diffuse2*0.2-0.15;
	
	return (1+fAmbient)*saturate(final); //+diffuse*diffuse*diffuse*diffuse*0.25
} 

technique DiffuseTechnique 
{ 
	pass P0 
	{ 
		ZWriteEnable = true;
		AlphaBlendEnable=False;
		VertexShader = compile vs_3_0 DiffuseVS(); 
		PixelShader  = compile ps_3_0 DiffusePS(); 
	} 
} 
