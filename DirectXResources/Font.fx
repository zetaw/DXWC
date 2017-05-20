Texture2D FontTexture : register(t0);
SamplerState samLinear : register(s0);

struct VSSceneIn{
	float3 pos : POSITION;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

struct PSSceneIn{
	float4 pos : SV_Position;
	float4 color : COLOR;
	float2 tex : TEXCOORD;
};

PSSceneIn VS(VSSceneIn input){
	PSSceneIn output = (PSSceneIn)0.0;
	output.pos = float4(input.pos, 1);
	output.tex = input.tex;
	output.color = input.color;
	return output;
}

// Pixel shader  
float4 PS(PSSceneIn input) : SV_Target
{
	float4 color = FontTexture.Sample(samLinear, input.tex);
	color.rgb = 1; 
	return color* input.color;
}