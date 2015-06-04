float4x4 matWorldViewProj;
float4x4 matWorld;
float4 vecAmbient;
float4 vecSunDir;
float3 flashlight_start_var;
float3 flashlight_end_var;
float flashlight_range_var;

texture entSkin1;
texture mtlSkin1;

sampler ColorMapSampler = sampler_state 
{ 
	Texture = <entSkin1>; 
	AddressU  = Wrap; 
	AddressV  = Wrap; 
}; 

sampler FLMaskSampler = sampler_state 
{ 
	Texture = <mtlSkin1>; 
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
in float3 pixPos: TEXCOORD2): COLOR 
{ 
	InNormal = normalize(InNormal);
	float4 TexColor = tex2D(ColorMapSampler, InTex);
	//float Diffuse = 0.1+0.1*saturate(dot(-vecSunDir, normalize(InNormal)));
	float Flashlight = 0;

	float3 pdir = pixPos-flashlight_start_var.xzy;
	float len = length(pdir);
	float3 ldir = flashlight_end_var.xzy-flashlight_start_var.xzy;
	float dirdot = dot(normalize(pdir),normalize(ldir));
	float Diffuse = saturate(dot(-normalize(ldir), normalize(InNormal)));
	
	float fac = saturate((flashlight_range_var-len)/flashlight_range_var);
	if(fac*dirdot*Diffuse > 0)
	{
		//ldiffuse = saturate(dot(-ldir, InNormal)*0.025+fac);
		float FLMask = tex2D(FLMaskSampler, float2(1-(dirdot-0.9)*10,0.5)).x;
		Flashlight = FLMask;
	}
	
	float ambdiffx = saturate(saturate(pixPos.x+20)+saturate(-407-pixPos.x));
	float ambdiffy = saturate((372-pixPos.z)*0.05);
	float flashlightfac = 1-saturate(ambdiffx+ambdiffy+saturate(pixPos.y-198));
	float Ambient = 0.75*saturate(dot(-vecSunDir, normalize(InNormal)))+0.5;
	Ambient = lerp(Ambient,0.125,flashlightfac);
	float4 final = TexColor*(Ambient+Flashlight*(0.2+0.8*flashlightfac));
	
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

