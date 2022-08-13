cbuffer ModelViewProjectionCB : register(b0)
{
	matrix MVP;
};

struct VertexPosColor
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct VertexShaderOutput
{
	float4 color : COLOR;
	float4 position : SV_Position;
};

VertexShaderOutput main(VertexPosColor input)
{
	VertexShaderOutput output;
	output.color = float4(input.color, 1.0f);
	output.position = mul(MVP, float4(input.position, 1.0f));
	return output;
}