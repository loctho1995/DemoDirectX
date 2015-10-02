/*

% Description of my shader.
% Second line of description for my shader.

keywords: material classic

date: YYMMDD

*/

float4x4 WorldViewProj : WorldViewProjection;

struct VS_INPUT
{
	float3 Position : POSITION0;
	float3 Color : COLOR0;
};

struct VS_OUTPUT
{
	float4 NewPosition : POSITION0;
	float InterpolatedColor : COLOR0;
};

VS_OUTPUT mainVS(VS_INPUT vsIn)
{
	VS_OUTPUT outp;
	outp.NewPosition = mul(float4(vsIn.Position.xyz, 1.0), WorldViewProj);
	outp.InterpolatedColor = vsIn.Color;
	return outp;
}

float4 mainPS(VS_OUTPUT pIn) : COLOR
{	
	return pIn.InterpolatedColor;
}

technique technique0 {
	pass p0 {
		CullMode = None;
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
