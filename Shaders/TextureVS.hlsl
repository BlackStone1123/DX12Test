struct PositionMatrix
{
	matrix model;
	matrix modelView;
	matrix modelViewProj;
	matrix InverseTransposeModelViewMatrix;
};

struct VSOut
{
	float2 tex : PixelTexCoord;
	float4 pos : SV_Position;
};

ConstantBuffer<PositionMatrix> modelViewProjectionCB: register(b0);

VSOut main( float3 pos : POSITION,float2 tex : TEXCOORD )
{
	VSOut vso;
	vso.pos = mul(modelViewProjectionCB.modelViewProj, float4(pos, 1.0f));
	vso.tex = tex;
	return vso;
}