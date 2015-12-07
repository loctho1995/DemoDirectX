/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/
struct VS_IN
{
	float3 Pos: POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD0;
};

struct VS_OUT
{
	float4 Pos: POSITION;
	float4 Color: COLOR;
	float2 TexCoord: TEXCOORD;
};


float4x4 WorldViewProj : WorldViewProjection;
Texture2D texture0;
float timer : register(c9);

SamplerState MeshTextureSampler
{
	Texture = <texture0>;
	AddressU = CLAMP;
    AddressV = CLAMP;
};

VS_OUT doVS(VS_IN input)
{
	VS_OUT output;
	output.Pos = mul(float4(input.Pos.xyz, 1.0), WorldViewProj);
	output.TexCoord = input.Pos.xyz;
	output.Color = input.Color;
	
	return output;
}

float4 doPX(VS_OUT input) : COLOR
{
/*
	D3DX9 
	Center: 0, 0
	Top-Left: -1, -1
	Bot-Right: 1, 1
	*/
	float2 pos = input.TexCoord.xy;
	
	if(pos.x <= 0)
	{
		pos = float2((1.0 - abs(pos.x)) / 2.0, pos.y);
	}
	else
	{
		pos = float2((1.0 + pos.x) / 2.0, pos.y);
	}
	
	if(pos.y <= 0)
	{
		pos = float2(pos.x, (1.0 - abs(pos.y)) / 2.0);
	}
	else
	{
		pos = float2(pos.x, (1.0 + pos.y) / 2.0);
	}
	
	pos = float2(pos.x, 1.0 - pos.y);
	
	input.Color = tex2D(MeshTextureSampler, pos);
	return input.Color * float4(0.3,0.1, 1.0, 0.2);
}

technique technique0 {
	pass p0 {
		CullMode = None;

		VertexShader = compile vs_3_0 doVS();
		PixelShader = compile ps_3_0 doPX();
	}
}
